from periphery import I2C
import time
import numpy as np
import copy
import struct

class HTPA:


	def __init__(self, address):
		self.address = address
		self.i2c = I2C("/dev/i2c-1")
		print("Grabbing EEPROM data")
		eeprom = self.get_eeprom()
		self.extract_eeprom_parameters(eeprom)
		self.eeprom = eeprom
		wakeup_and_blind = self.generate_command(0x01, 0x01)  # wake up the device

		# ~ adc_res = self.generate_command(0x03, 0x0C) # set ADC resolution in eeprom
		# ~ pull_ups = self.generate_command(0x09,0x88 ) #pu value in eeprom
		# set ADC resolution to 16 bits
		adc_res = self.generate_command(0x03, self.mbit_value)
		pull_ups = self.generate_command(0x09, self.pu_value)

		print("Initializing capture settings")

		self.send_command(wakeup_and_blind)
		self.send_command(adc_res)
		self.send_command(pull_ups)

		self.set_bias_current(self.bias_value)  # bias value on eeprom
		self.set_clock_speed(0x050)  # clk value on eeprom self.clk_value
		self.set_cm_current(self.bpa_value)  # BPA value in eeprom

		# initialize offset to zero
		self.offset = np.zeros((32, 32))



	def get_eeprom(self, eeprom_address=0x50):#Talking EEPROM
		query = [I2C.Message([0x00, 0x00]), I2C.Message(
		    [0x00]*8000, read=True)]  # 8 Kbit Data from EEPROM
		self.i2c.transfer(eeprom_address, query)
		return np.array(query[1].data)


	def extract_eeprom_parameters(self, eeprom):#EEPROM Data
		self.VddCompgrad = eeprom[0x0340:0x0540:2] + (eeprom[0x0341:0x0540:2] << 8)
		self.VddCompoff = eeprom[0x0540:0x0740:2] + (eeprom[0x0541:0x0740:2] << 8)

		ThGrad = eeprom[0x0740:0x0F40:2] + (eeprom[0x0741:0x0F40:2] << 8)
		ThGrad = [tg - 65536 if tg >= 32768 else tg for tg in ThGrad]
		ThGrad = np.reshape(ThGrad, (32, 32))
		ThGrad[16:, :] = np.flipud(ThGrad[16:, :])
		self.ThGrad = ThGrad

		ThOffset = eeprom[0x0F40:0x1740:2] + (eeprom[0x0F41:0x1740:2] << 8)
		ThOffset = np.reshape(ThOffset, (32, 32))
		ThOffset[16:, :] = np.flipud(ThOffset[16:, :])
		self.ThOffset = ThOffset

		P = eeprom[0x1740::2] + (eeprom[0x1741::2] << 8)
		P = np.reshape(P, (32, 32))
		P[16:, :] = np.flipud(P[16:, :])
		self.P = P

		epsilon = float(eeprom[0x000D])
		GlobalGain = eeprom[0x0055] + (eeprom[0x0056] << 8)
		Pmin = eeprom[0x0000:0x0004]
		Pmax = eeprom[0x0004:0x0008]
		Pmin = struct.unpack('f', reduce(
		    lambda a, b: a+b, [chr(p) for p in Pmin]))[0]
		Pmax = struct.unpack('f', reduce(
		    lambda a, b: a+b, [chr(p) for p in Pmax]))[0]
		self.PixC = (P * (Pmax - Pmin) / 65535. + Pmin) * \
		             (epsilon / 100) * float(GlobalGain) / 100
		self.gradScale = eeprom[0x0008]
		self.VddCalib1 = eeprom[0x0046] + (eeprom[0x0047] << 8)
		self.VddCalib = eeprom[0x0046] + (eeprom[0x0047] << 8)
		self.VddCalib2 = eeprom[0x0048] + (eeprom[0x0049] << 8)
		self.Vdd = 3000.0
		self.VddScaling = eeprom[0x004E]
		self.Vddoff = eeprom[0x004F]

		self.PtatCalib1 = eeprom[0x003C] + (eeprom[0x003D] << 8)
		self.PtatCalib2 = eeprom[0x003E] + (eeprom[0x003F] << 8)
		PTATgradient = eeprom[0x0034:0x0038]
		self.PTATgradient = struct.unpack('f', reduce(
		    lambda a, b: a+b, [chr(p) for p in PTATgradient]))[0]
		PTAToffset = eeprom[0x0038:0x003c]
		self.PTAToffset = struct.unpack('f', reduce(
		    lambda a, b: a+b, [chr(p) for p in PTAToffset]))[0]
		self.clk_value = eeprom[0x001C]
		self.bias_value = eeprom[0x001B]
		self.pu_value = eeprom[0x001E]
		self.mbit_value = eeprom[0x001A]
		self.bpa_value = eeprom[0x001D]
		self.subt = np.zeros((32, 32))
		
		
	def set_clock_speed(self, clk):#set clock speed
		if clk > 63:  # Max 64 Hz
			clk = 63
		if clk < 0:
			clk = 0
		clk = int(clk)
		print(clk)
		# The measure time depends on the clock frequency settings.(optimal value)
		clk_speed = self.generate_command(0x06, clk)
		self.send_command(clk_speed)  # send clock data

	# This setting is used to adjust the common mode voltage of the preamplifier.

	
	def set_cm_current(self, cm):
		cm = int(cm)
		cm_top = self.generate_command(0x07, cm)
		cm_bottom = self.generate_command(0x08, cm)

		self.send_command(cm_top)
		self.send_command(cm_bottom)


	def set_bias_current(self, bias):
		bias = int(bias)
		# This setting is used to adjust the bias current of the ADC. A faster clock frequency requires a higher bias current setting.
		bias_top = self.generate_command(0x04, bias)
		# This setting is used to adjust the bias current of the ADC. A faster clock frequency requires a higher bias current setting.
		bias_bottom = self.generate_command(0x05, bias)
		self.send_command(bias_top)  # send bias top data
		self.send_command(bias_bottom)  # send bias bottom data


    def temperature_compensation(self, im, ptat):#Thermal Offset Calculate
	    comp = np.zeros((32,32))
	    Ta = np.mean(ptat) * self.PTATgradient + self.PTAToffset
		#     temperature compensated voltage
	    comp = ((self.ThGrad * Ta) / pow(2, self.gradScale)) + self.ThOffset
	    Vcomp = np.reshape(im,(32, 32)) - comp
	    return Vcomp

	def offset_compensation(self, im):#general environment offset send offset data
		return im-self.offset
		
	def sensitivity_compensation(self, im):#object temperature 
		return (im*100000000)/self.PixC

	def measure_observed_offset(self):#Measuring observed offsets
		mean_offset = np.zeros((32, 32))
		for i in range(10):
			print("    frame " + str(i))
			(p, pt) = self.capture_image()
			im = self.temperature_compensation(p, pt)
			mean_offset += (im-10)/10.0	
		self.offset = mean_offset


	def Vdd_Comperasition(self,im,ptat):#Vdd Comperasition calculate
		VVddComp=[]
		for i in range(16):
			for j in range(32):
				VVddComp.append((((self.VddCompgrad[(j+i*32)%128]*np.mean(ptat))/pow(2, self.VddScaling)+self.VddCompoff[(j+i*32)%128])/pow(2, self.Vddoff))*(self.Vdd-self.VddCalib1-((self.VddCalib2-self.VddCalib1)/(self.PtatCalib2-self.PtatCalib1))*(np.mean(ptat)-self.PtatCalib1)))
		for i in range(16,32):
			for j in range(32):
				VVddComp.append((((self.VddCompgrad[(j+i*32)%128+128]*np.mean(ptat))/pow(2, self.VddScaling)+self.VddCompoff[(j+i*32)%128+128])/pow(2, self.Vddoff))*(self.Vdd-self.VddCalib1-((self.VddCalib2-self.VddCalib1)/(self.PtatCalib2-self.PtatCalib1))*(np.mean(ptat)-self.PtatCalib1)))
		self.VVddComp=VVddComp
		return im-np.reshape(self.VVddComp,(32, 32))

	def measure_electrical_offset(self, blind=True):#measure_electrical_offset
		pixel_values = np.zeros(256)
		ptats = np.zeros(8)
        
        self.send_command(self.generate_expose_block_command(0, blind=blind), wait=False)

		query = [I2C.Message([0x02]), I2C.Message([0x00], read=True)]

		read_block = [I2C.Message([0x0A]), I2C.Message([0x00]*258, read=True)]
		self.i2c.transfer(self.address, read_block)
		top_data = np.array(copy.copy(read_block[1].data))

		read_block = [I2C.Message([0x0B]), I2C.Message([0x00]*258, read=True)]
		self.i2c.transfer(self.address, read_block)
		bottom_data = np.array(copy.copy(read_block[1].data))

		top_data = top_data[1::2] + (top_data[0::2] << 8)
		bottom_data = bottom_data[1::2] + (bottom_data[0::2] << 8)
        # bottom data is in a weird shape
		pixel_values[0:128] = top_data[1:]
		# bottom data is in a weird shape
		pixel_values[224:256] = bottom_data[1:33]
		pixel_values[192:224] = bottom_data[33:65]
		pixel_values[160:192] = bottom_data[65:97]
		pixel_values[128:160] = bottom_data[97:]
		ptats[block] = top_data[0]
		ptats[7-block] = bottom_data[0]

        self.elloff=pixel_values;


	def electrical_offset(self,im):#electrical offset calculate
        V_new = np.zeros((32,32))
	    for i in range(16):
	        for j in range(32):
                V_new[i,j]=self.elloff[(j+i*32)%128]
	    for i in range(16,32):
	        for j in range(32):
                V_new[i,j]=self.elloff[(j+i*32)%128+128]
        self.V_new=V_new
		return im - self.V_new
	def capture_image(self, blind=False):
		pixel_values = np.zeros(1024)
		ptats = np.zeros(8)

		for block in range(4):
			print("Exposing block " + str(block))
			self.send_command(self.generate_expose_block_command(block, blind=blind), wait=False)

			query = [I2C.Message([0x02]), I2C.Message([0x00], read=True)]
			expected = 1 + (block << 4)

			done = False

			while not done:
				self.i2c.transfer(self.address, query)

				if not (query[1].data[0] == expected):
					# print("Not ready, received " + str(query[1].data[0]) + ", expected " + str(expected))
					#time.sleep(0.03)#Wait 30 ms
				else:
					done = True

			read_block = [I2C.Message([0x0A]), I2C.Message([0x00]*258, read=True)]
			self.i2c.transfer(self.address, read_block)
			top_data = np.array(copy.copy(read_block[1].data))

			read_block = [I2C.Message([0x0B]), I2C.Message([0x00]*258, read=True)]
			self.i2c.transfer(self.address, read_block)
			bottom_data = np.array(copy.copy(read_block[1].data))

			top_data = top_data[1::2] + (top_data[0::2] << 8)
			bottom_data = bottom_data[1::2] + (bottom_data[0::2] << 8)

			pixel_values[(0+block*128):(128+block*128)] = top_data[1:]
			# bottom data is in a weird shape
			pixel_values[(992-block*128):(1024-block*128)] = bottom_data[1:33]
			pixel_values[(960-block*128):(992-block*128)] = bottom_data[33:65]
			pixel_values[(928-block*128):(960-block*128)] = bottom_data[65:97]
			pixel_values[(896-block*128):(928-block*128)] = bottom_data[97:]

			ptats[block] = top_data[0]
			ptats[7-block] = bottom_data[0]

		pixel_values = np.reshape(pixel_values, (32, 32))

		return (pixel_values, ptats)


	def generate_command(self, register, value):#periphery library register activate
		return [I2C.Message([register, value])]


	def generate_expose_block_command(self, block, blind=False):#read data command
		if blind:
			return self.generate_command(0x01, 0x0B)
		else:
			return self.generate_command(0x01, 0x09 + (block << 4))


	def send_command(self, cmd, wait=True):#send data to registers
		self.i2c.transfer(self.address, cmd)
		if wait:
			time.sleep(0.005) # sleep for 5 ms


	def close(self):#closed device
		sleep = self.generate_command(0x01, 0x00)
		self.send_command(sleep)

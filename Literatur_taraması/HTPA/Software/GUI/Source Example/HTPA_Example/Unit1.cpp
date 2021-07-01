//---------------------------------------------------------------------------

#define DONT_USE_WINSPOOL_SETPORTA
#define DISABLE_TOHEX_IN_VFWh


#include <vcl.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <vfw.h>
#include <float.h>
#include <windows.h>
#include <shellapi.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit5.h"
#include "Unit8.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//globals
UnicodeString NetworkIF,localIP;
TStrings *Interfaces;//Strings of Networkinterfaces
signed int nrofclients=0,actselecteddev=0;
deviceconsts DevConst[MAXNROFIPS];
TIdSocketHandle *CamBinding;
bool Clientfound=false;
AnsiString actPeerIP;
unsigned int PIXEL,OPAS,ZEILE,SPALTE,SENSPIX,EEPROMLENGTH,ROWCOLSIZE=66;
unsigned int ReceiveBuffer[4160],VDD,AMB;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
IdUDPServer1->OnUDPRead=IdUDPServer1UDPRead;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, TIdBytes AData,
		  TIdSocketHandle *ABinding)
{
unsigned int length,i,k,kk;
unsigned long t;
UnicodeString UStemp;
AnsiString peer,ndtemp,ASCamIP,ASPCIP,AScheck,listeddev,AStemp;
deviceconsts TempConst;
bool alreadyknown=false,checkvar,binarypaket;
double realkHz;
char ctemp[1100],cc;
static bool firstdataset,datasets64[8],lost64pack;

length=AData.get_length();
AStemp=BytesToString(AData,0,length);

if((PIXEL*2==length)||
	( PIXEL==1056&&((length==1054)||(length==1058)))||
	((PIXEL==4096)&&((length==1101)||(length==493))))
	binarypaket=true;

if(AStemp.SubString(1,22)=="HTPA series responsed!"){
	TempConst.devType=0;
	TempConst.devkHz=0;
	TempConst.GAIN=false;
	TempConst.THvsTA=false;
	TempConst.PixCvsTA=false;
	TempConst.F8_14=false;
	TempConst.BFL3=false;
	TempConst.IGNORE_ELOFF=false;
	TempConst.ELOFF32=false;
	TempConst.SBY=false;
	TempConst.FLOATCALIB=false;
	TempConst.STEFBOLEXP=4.00;
	TempConst.selected=false;
	TempConst.subframe=0;
	TempConst.running=false;
	for(i=0;i<nrofclients;i++){    //new device?
		if(ABinding->PeerIP==DevConst[i].DEVICEIP){
			alreadyknown=true;
			break;
			}
		}
 /*	if(!Form5->CheckBox1->Checked)
		ABinding->Connect();    */
	TempConst.DEVICEIP=ABinding->PeerIP;
	if((nrofclients)&&(!alreadyknown)){
		Edit1->Text="Multiple clients online. GUI selects device which answers first.";
		ShowMessage("More than one client found! Select device via Options.");
	 //	actselecteddev++;
		}
	if(AStemp.SubString(23,17)==" I am Arraytype 0"){
		peer=(AnsiString)ABinding->PeerIP+"  8x8  ";
		TempConst.devType=1;
		}
	if(AStemp.SubString(23,17)==" I am Arraytype 1"){
		peer=(AnsiString)ABinding->PeerIP+"  16x16";
		TempConst.devType=2;
		}
	if(AStemp.SubString(23,17)==" I am Arraytype 2"){
		peer=(AnsiString)ABinding->PeerIP+"  16x32";
		TempConst.devType=3;
		}
	if(AStemp.SubString(23,17)==" I am Arraytype 3"){
		peer=(AnsiString)ABinding->PeerIP+"  32x31";
		TempConst.devType=4;
		}
	if(AStemp.SubString(23,17)==" I am Arraytype 5"){
		peer=(AnsiString)ABinding->PeerIP+"  64x62";
		TempConst.devType=6;
		}
	if(!nrofclients){  //first answer from a device
		TempConst.selected=true;
		}
	else
		TempConst.selected=false;
	nrofclients++;
	i=AStemp.Pos("I am running on ");
	if(i){
		ndtemp=AStemp.SubString(i+16,6);
		realkHz=ndtemp.ToDouble();
		TempConst.devkHz=realkHz;
		}
	i=AStemp.Pos("Amplification is ");
	if(i){
		if(AStemp.SubString(i+17,3)=="low"){
			peer+=" L";
			TempConst.GAIN=false;
			}
		else{
			peer+=" H";
			TempConst.GAIN=true;
			}
		}
 	i=AStemp.Pos("MAC-ID: ");
	if(i){
		TempConst.MACID=AStemp.SubString(i+8,17);
		for(k=0;k<5;k++){
			i=TempConst.MACID.Pos(".");
			TempConst.MACID.Delete(i,1);
			TempConst.MACID.Insert(":",i);
			}
		peer+=(" ["+TempConst.MACID.SubString(10,8)+"]");
		}

	//now check overwrite old values or create a new device
	if(alreadyknown){
		nrofclients--;
		for(i=0;i<nrofclients;i++){
			if(TempConst.DEVICEIP==DevConst[i].DEVICEIP){
				DevConst[i]=TempConst;
				break;
				}
			}
		kk=i;
		for(i=0;i<(Form5->ComboBox1->Items->Count);i++){
			Form5->ComboBox1->ItemIndex=i;
			if(ABinding->PeerIP==Form5->ComboBox1->Text.SubString(1,15)){
				Form5->ComboBox1->Items->Delete(i);
				Form5->ComboBox1->Items->Add(peer);
				t=GetTickCount();
				if((!DevConst[kk].running)){   //only bind if it doesnt run
					while((t+5)>GetTickCount());
					IdUDPServer1->Send(ABinding->PeerIP,30444,"Bind HTPA series device");
					}
				break;
				}
			}
		}
	else{
		DevConst[nrofclients-1]=TempConst;
		Form5->ComboBox1->Items->Add(peer);
		IdUDPServer1->Send(ABinding->PeerIP,30444,"Bind HTPA series device");
		}
	Form5->ComboBox1->Update();
	Form5->ComboBox1->Invalidate();
	Form5->ComboBox1->ItemIndex=actselecteddev;
	ASCamIP=ABinding->PeerIP;
	ASPCIP=localIP;
	checkvar=true;
	for(i=0;i<3;i++){   //check the first three bytes, assuming subnetmask is 255.255.255.000
		k=ASCamIP.Pos(".");
		kk=ASPCIP.Pos(".");
		if(ASCamIP.SubString(1,k-1)!=ASPCIP.SubString(1,kk-1))
			checkvar=false;
		ASCamIP.Delete(1,k);
		ASPCIP.Delete(1,kk);
		}
	if(checkvar){
		CamBinding=ABinding;
		Edit1->Text="Client found at "+CamBinding->PeerIP+". Ready to operate";
		}
	else{
		 Edit1->Text="Client found at "+ABinding->PeerIP+". Maybe the Recipient is unrechable!!!";
		 ShowMessage("Maybe the Recipient is unreachable! IP of device can be configured by typing i.e. 111.111.111.111.255.255.255.000.setIP or by use of Options");
		 }
	Clientfound=true;
	actPeerIP=DevConst[actselecteddev].DEVICEIP;
//	sprintf(ctemp,"%6.1f",DevConst[actselecteddev].devkHz);
  //	Edit6->Text=(AnsiString)ctemp;
	Edit1->Text="Actual selected device "+DevConst[actselecteddev].DEVICEIP;
	Label2->Caption="Device IP: "+DevConst[actselecteddev].DEVICEIP;
	}


if(binarypaket){
	if(DevConst[actselecteddev].devType==4){
		if(((length==1054)&&(!firstdataset))||((length==1058)&&(firstdataset))){
	#ifdef FIRMWARE
			Edit1->Text="Frame lost.";
	#endif
			return;
			}
		if((length==1058)&&(!firstdataset)){
			for(i=0;i<(length/2);i++)
				ReceiveBuffer[i]=(unsigned int)BytesToWord(AData,i*2);
			firstdataset=true;
			return;
			}
		if((length==1054)&&(firstdataset)){
			for(i=0;i<(length/2);i++)
				ReceiveBuffer[i+529]=(unsigned int)BytesToWord(AData,i*2);
			firstdataset=false;
			}
		VDD=ReceiveBuffer[1024]|(ReceiveBuffer[1025]<<12);
		AMB=ReceiveBuffer[1026]|(ReceiveBuffer[1027]<<12);
		}
	else if(DevConst[actselecteddev].devType==5){
		cc=BytesToChar(AData,0);
		if(lost64pack&&(cc!=1))
			return;
		if(lost64pack&&(cc==1))
			lost64pack=false;
		if((cc<=8)&&(cc>=1)){ //Ok, it was a datapacket..
			if(datasets64[cc-1]){
	#ifdef FIRMWARE
				Edit1->Text="Frame lost.";
	#endif
				for(i=0;i<8;i++)
					datasets64[i]=false;
				return;
				}
			if(cc>=2){
				if(!datasets64[cc-2]){	//lost a dataset
					for(i=0;i<8;i++)
						datasets64[i]=false;
					lost64pack=true;
					return;
					}
				}

			datasets64[cc-1]=true;
			for(i=0;i<((length-1)/2);i++){
				ReceiveBuffer[i+((cc-1)*550)]=(unsigned int)BytesToWord(AData,(i*2)+1);
				}
			//now check, if all packets were received
			for(i=0;i<8;i++){
				if(!datasets64[i])
					return;
				}
			for(i=0;i<8;i++)
				datasets64[i]=false;
			VDD=ReceiveBuffer[4032]|(ReceiveBuffer[4033]<<12);
			AMB=ReceiveBuffer[4034]|(ReceiveBuffer[4035]<<12);
			}
		}
	}
	Edit1->Text="VDD is "+(UnicodeString)VDD+" and TA is"+(UnicodeString)AMB;


}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
unsigned int t,i;
AnsiString sub;
static bool Interfacewarning;
TIdStackSocketHandle *A=new  TIdStackSocketHandle;
AnsiString AStemp;
char ltemp[100];


if(Sender==Form8->Button1){
	TIdSocketHandle *SockH=IdUDPServer1->Bindings->Add();
	IdUDPServer1->Active=false;
	IdUDPServer1->Bindings->Items[0]->IP=NetworkIF;
	IdUDPServer1->Bindings->Items[0]->Port=30444;
	IdUDPServer1->Bindings->Items[0]->IPVersion=Id_IPv4;
	}

Interfaces=GStack->LocalAddresses;
if((Interfaces->Count>1)&&(!Interfacewarning)){
	Interfacewarning=true;

	for(i=0;i<Interfaces->Count;i++){
		AStemp=(AnsiString)Interfaces->Strings[i];
		Form8->ComboBox1->Items->Add((UnicodeString)AStemp);
		if(!i)
			Form8->ComboBox1->Text=(UnicodeString)AStemp;
		}
	Form8->Show();
	Form1->Enabled=false;
	return;
	}
/*________________alter text_____________________*/
IdUDPServer1->Active=true;
if(Interfaces->Count==1)
	localIP=GStack->LocalAddress;
else if(Interfaces->Count>1)
	localIP=IdUDPServer1->Bindings->Items[0]->IP;
else{	//no Network interface found!!!
	ShowMessage("No network interface found on this machine!!!");
	return;
	}
Label1->Caption="Local IP: "+localIP;


sub="Calling HTPA series devices";
IdUDPServer1->Broadcast(sub,30444);
Label2->Caption="Device IP: Searching...";

return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
Form5->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Initvarias(unsigned int type){
float h,w;

if(type==1){ //9x8
	PIXEL=72;      		//Nr of datsets
	SENSPIX=64;			//sensitive pixels
	OPAS=4;
	ZEILE=9;
	SPALTE=8;
	}
if(type==2){   //16x17
	PIXEL=272;
	SENSPIX=256;			//sensitive pixels
	OPAS=8;
	ZEILE=17;
	SPALTE=16;
	}
if(type==3){   //16x32
	PIXEL=544;
	OPAS=16;
	ZEILE=17;
	SPALTE=32;
	}
if(type==4){   //32x32
	PIXEL=1056;
	SENSPIX=992;			//sensitive pixels
	OPAS=16;
	ZEILE=33;
	SPALTE=32;
	}
if(type==5){	//Zeile64
	PIXEL=72;
	SENSPIX=64;			//sensitive pixels
	SPALTE=64;
	ZEILE=1;
	}

if(type==6){   //64x62
	PIXEL=4096;
	SENSPIX=3968;			//sensitive pixels
	OPAS=32;
	ZEILE=64;
	SPALTE=64;
	}
if(type==7){	//16x4
	PIXEL=65;      		//Nr of datsets
	SENSPIX=64;			//sensitive pixels
	OPAS=8;
	ZEILE=5;
	SPALTE=16;

	}

if(type==8){	//HID
	PIXEL=1;      		//Nr of datsets
	SENSPIX=1;			//sensitive pixels
	OPAS=1;
	ZEILE=1;
	SPALTE=1;

	}



return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
IdUDPServer1->Send(DevConst[actselecteddev].DEVICEIP,30444,"K");
}
//---------------------------------------------------------------------------


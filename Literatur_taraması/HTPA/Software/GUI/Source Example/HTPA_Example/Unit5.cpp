//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;



extern AnsiString actPeerIP;
extern signed int nrofclients,actselecteddev;
extern deviceconsts DevConst[MAXNROFIPS];


//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
void __fastcall TForm5::FormShow(TObject *Sender)
{
AnsiString AStemp;
unsigned int i,k,xoff,yoff;
TPoint *temppnt2 = new TPoint;
TCanvas *zflaech = new TCanvas();

Form1->Enabled=false;


Form5->ComboBox1->ItemIndex=actselecteddev;
for(i=0;i<Form5->ComboBox1->Items->Count;i++){
	AStemp=Form5->ComboBox1->Items->operator [](i);
	if(AStemp.SubString(1,15)==DevConst[actselecteddev].DEVICEIP)
		Form5->ComboBox1->ItemIndex=i;
	}


}
//---------------------------------------------------------------------------






void __fastcall TForm5::Edit1KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------


void __fastcall TForm5::Edit2KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit3KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit4KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit5KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit6KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit7KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit8KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit1->Text.Length())){
   Edit1->Text="192";
   }

temp=Edit1->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit2KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit2->Text.Length())){
   Edit2->Text="168";
   }

temp=Edit2->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit3KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit3->Text.Length())){
   Edit3->Text="240";
   }

temp=Edit3->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit4KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit4->Text.Length())){
   Edit4->Text="122";
   }

temp=Edit4->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit5KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit5->Text.Length())){
   Edit5->Text="255";
   }

temp=Edit5->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit6KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit6->Text.Length())){
   Edit6->Text="255";
   }

temp=Edit6->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit7KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit7->Text.Length())){
   Edit7->Text="255";
   }

temp=Edit7->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit8KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
AnsiString temp;

if((Key==8)&&(!Edit8->Text.Length())){
   Edit8->Text="000";
   }

temp=Edit8->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button1Click(TObject *Sender)
{
AnsiString temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,AStemp;
char txt[50];

temp1=Edit1->Text;
temp2=Edit2->Text;
temp3=Edit3->Text;
temp4=Edit4->Text;
temp5=Edit5->Text;
temp6=Edit6->Text;
temp7=Edit7->Text;
temp8=Edit8->Text;

if(temp1.Length()!=3)                  //first check format
	Edit1->Color=clRed;
else
	Edit1->Color=clWindow;
if(temp2.Length()!=3)
	Edit2->Color=clRed;
else
	Edit2->Color=clWindow;
if(temp3.Length()!=3)
	Edit3->Color=clRed;
else
	Edit3->Color=clWindow;
if(temp4.Length()!=3)
	Edit4->Color=clRed;
else
	Edit4->Color=clWindow;
if(temp5.Length()!=3)
	Edit5->Color=clRed;
else
	Edit5->Color=clWindow;
if(temp6.Length()!=3)
	Edit6->Color=clRed;
else
	Edit6->Color=clWindow;
if(temp7.Length()!=3)
	Edit7->Color=clRed;
else
	Edit7->Color=clWindow;
if(temp8.Length()!=3)
	Edit8->Color=clRed;
else
	Edit8->Color=clWindow;

if((Edit1->Color==clRed)||(Edit2->Color==clRed)||(Edit3->Color==clRed)||(Edit4->Color==clRed)||(Edit5->Color==clRed)||(Edit6->Color==clRed)||(Edit7->Color==clRed)||(Edit8->Color==clRed)){
	ShowMessage("Figure format is 3 digits. Please type i.e. 001");
	return;
	}
ShowMessage("Be aware that only one device is online! Otherwise all devices will change their IP to the given value!");
#ifdef FIRMWARE
AStemp="Cam.exe begs you to change IP to "+temp1+"."+temp2+"."+temp3+"."+temp4+"."+temp5+"."+temp6+"."+temp7+"."+temp8+".";
Form1->UDPServer1->Broadcast(AStemp,30444);
#endif
AStemp="HTPA device IP change request to "+temp1+"."+temp2+"."+temp3+"."+temp4+"."+temp5+"."+temp6+"."+temp7+"."+temp8+".";
Form1->IdUDPServer1->Broadcast(AStemp,30444);

Form5->Timer1->Interval=3000;
Form5->Timer1->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm5::Button2Click(TObject *Sender)
{
AnsiString AStemp;
signed int k,i;
unsigned long ticks;


actPeerIP=ComboBox1->Text.SubString(1,15);
actselecteddev=ComboBox1->Items->IndexOf(ComboBox1->Text);
ComboBox1->Items->Delete(actselecteddev);


//now tell it the device
for(i=0;i<10;i++){
	AStemp="x Release HTPA series device";
	Form1->IdUDPServer1->Send(actPeerIP,30444,AStemp);
	ticks=GetTickCount();
	while((ticks+10)>(GetTickCount()));
	}
//now set a new actual device.
ComboBox1->Items->operator [](0);
ComboBox1->ItemIndex=0;
ComboBox1Change(Button2);
}
//---------------------------------------------------------------------------




void __fastcall TForm5::Edit10KeyPress(TObject *Sender, wchar_t &Key)
{
Key = (((Key >= '0') && (Key <= '9')) || (Key==8) ) ? Key:0;
}
//---------------------------------------------------------------------------


void __fastcall TForm5::Timer1Timer(TObject *Sender)
{
Edit9->Text="Recipient unreachable.";
}
//---------------------------------------------------------------------------

void __fastcall TForm5::ComboBox1Change(TObject *Sender)
{
AnsiString t,AStemp;
unsigned int k,i;
char ctemp[10];


i=ComboBox1->Text.Pos(" ")+2;   //now set Moduletype
k=ComboBox1->Text.Length()+1;
t=ComboBox1->Text.SubString(i,k-i-2);
if(t.Pos("8x8  "))Form1->Initvarias(1);
if(t.Pos("16x16"))Form1->Initvarias(2);
if(t.Pos("16x32"))Form1->Initvarias(3);
if(t.Pos("32x31"))Form1->Initvarias(4);
if(t.Pos("64x62"))Form1->Initvarias(6);
if(t.Pos("16x4"))Form1->Initvarias(7);
if(t.Pos("HID"))Form1->Initvarias(8);
actPeerIP=ComboBox1->Text.SubString(1,15);
//for(i=0;i<ComboBox1->Items->Count;i++){    //new device?
for(i=0;i<nrofclients;i++){
//	ComboBox1->ItemIndex=i;
	if(DevConst[i].DEVICEIP==actPeerIP){
		actselecteddev=i;
		DevConst[i].selected=true;
		}
	else
		DevConst[i].selected=false;
	}
for(i=0;i<ComboBox1->Items->Count;i++){
	if(ComboBox1->Items->operator[](i).SubString(1,15)==DevConst[actselecteddev].DEVICEIP){
		ComboBox1->ItemIndex=i;
		break;
		}
	}



Form1->Edit1->Text="Actual selected device "+DevConst[actselecteddev].DEVICEIP;



}
//---------------------------------------------------------------------------



void __fastcall TForm5::Button3Click(TObject *Sender)
{
AnsiString ASlocal;
ASlocal="\n\nHEIMANN Sensor GmbH - Headquarter\nGrenzstrasse 22   D-01109 Dresden\nhttp://www.heimannsensor.com/\n\nThis Software was designed to control Heimann Sensor's HTPA modules, including recording and visualization of their datastream.\n\nPortions of this software are Copyright (c) 1993 - 2003, Chad Z. Hower (Kudzu) and the Indy Pit Crew - http://www.IndyProject.org/\n\nWritten by B.Forg \nemail: forg@heimannsensor.com";
ShowMessage(ASlocal);
}
//---------------------------------------------------------------------------


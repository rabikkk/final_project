%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %     Copyright 2011 Dokuz Eylül University Electrical Electronics Engineering Department      
% %
% %     This program is free software: you can redistribute it and/or modify
% %     it under the terms of the GNU General Public License as published by
% %     the Free Software Foundation, either version 3 of the License, or
% %     (at your option) any later version.
% % 
% %     This program is distributed in the hope that it will be useful,
% %     but WITHOUT ANY WARRANTY; without even the implied warranty of
% %     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% %     GNU General Public License for more details.
% % 
% %     You should have received a copy of the GNU General Public License
% %     along with this program.  If not, see <http://www.gnu.org/licenses/>.
% %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %
% % Yazım tarihi: 2012/12/03 DEU Tez Döküman Sınıfı v1.0
% % Yazan : Yard. Doç. Dr. Serkan GÜNEL - Semih BAŞ
% %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %
% % Bu kod Dokuz Eylül Üniversitesi Mühendislik Fakültesi Elektrik Elektronik Mühendisliği Bölümü için tez dökümanı
% % oluşturulması için yazılmıştır. Bu kod ile birlikte tez dökümanın iç sayfalarında gerekli olan düzenlemeler, kapak 
% % sayfaları ve içindekiler sayfası kolaylıkla oluşturulmaktadır.
% %
% % Latex Dokumanı EED4093  Research Project dersi için \documentclass{deutez} ile başlamalıdır.
% %
% % Latex Dokumanı EED4094  Final Project dersi için \documentclass[thesis]{deutez} ile başlamalıdır.
% %
% % Bu sayfaların oluşturulması için bu döküman sınıfını kullanan kullanıcının belli başlı parametreleri girmesi gerekmektedir.
% % Bu parametreler açıklamak gerekirse
% % \watermarklogo{Deu.jpg} : Staj yapılan kurumun logosunu sayfaların arkasına filigran olarak yerleştirir.Ancak bu komutla 
% % 			  herhangi bir logo alınmaz ise döküman varsayılan değer olarak sayfaların arkasına Dokuz Eylül Üniversitesi
% % 			  logosunu basar.
% % \projectname{~} 		: Teze konu olan projenin ismi girilir.
% % \ogrencininadi{~} 		: Teze kayıtlanan öğrencinin adı girilir.
% % \advisor{~} 			: Tezin danışmanının adı ünvanı ile birlikte girilir.
% % \time{~} 				: Tezin yazıldığı tarih girilir.
% % \jurya{~} 				: Tez savunması yapılacak olan juri üyelerinden sadece 1 kişinin adı girilir.
% % \juryb{~} 				: Tez savunması yapılacak olan juri üyelerinden sadece 1 kişinin adı girilir.
% % \chair{~} 				: Bölüm başkanının adı girilir.
% % 
% % Bu döküman sınıfı ile oluşturulacak dökümanlarda logo ve fotoğraf dosyaları aynı dizin içinde bulunmalıdır.
% % Bu döküman sınıfı kullanılırken "\begin{document} \end{document}"  ortam belirteçlerinden sonra eğer kullanılacaksa                 
% %	"\begin{acknowledgements} \end{acknowledgements}", "\begin{abstract} \end{abstract}" ve "\begin{ozet} \end{ozet}" ortam belirteçleri 
% % kullanıldıktan sonra \tableofcontents, \listoffigures ve \listoftable komutları kullanılmalıdır. 
% % Dökümanı yazmya başlamadan önce \start komutu kullanılmalıdır. 
% % 
% % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Gerekli olan sınıf ve paket tanımlamaları
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{deutez}[2014/06/05 Tez Döküman Sınıfı v1.1]

\newif\if@thesis\@thesisfalse

\DeclareOption{thesis}{\@thesistrue}
\ProcessOptions
\PassOptionsToClass{a4paper,11pt,oneside}{report}
\LoadClass{report}
\RequirePackage[english]{babel} 
\RequirePackage[utf8]{inputenc}
\RequirePackage{graphicx}
\RequirePackage[top=3cm,left=4cm,right=2.5cm,bottom=3cm]{geometry}
\RequirePackage{everypage}
\RequirePackage{afterpage}
\RequirePackage{tikz}
%\makeatletter
%\global\let\tikz@ensure@dollar@catcode = \relax 
%\makeatother
\RequirePackage{ifthenx}
\RequirePackage{cite}
\usetikzlibrary{calc,shapes.multipart}

%Arkaplanın kapak sayfalarında ve içindekiler sayfasında kullanılmaması için gerekli olan kodlar.
\newif\ifbg@some
\bg@somefalse

%Arkaplanın kapak sayfalarında ve içindekiler sayfasında kullanılmaması için gerekli olan tanımlamalar.
\def\bg@contents{}
\def\bg@color{black!45}
\def\bg@angle{0}
\def\bg@opacity{0.5}
\def\bg@scale{1}
\def\bg@position{current page.center}
\def\bg@anchor{}
\def\bg@hshift{0}
\def\bg@vshift{0}

%Sınıf dosyası içinde kullanılan komut tanımları
\def\watermarklogo#1{\def\@logofile{#1}}%
\def\projectname#1{\def\@projectname{#1}}% Proje adı
\def\ogrencininadi#1{\def\@ogrencininadi{#1}}% Öğrencinin adı
\def\advisor#1{\def\@advisor{#1}}% Danışmanın ismi
\def\time#1{\def\@time{#1}}% Tezin yazım tarihi
\def\jurya#1{\def\@jurya{#1}}% Jüri üyesinin adı
\def\juryb#1{\def\@juryb{#1}}% Jüri üyesinin adı
\def\chair#1{\def\@chair{#1}}% Bölüm başkanın adı
\def\getsection#1{\def\@currentsection{#1}}%
\def\getchapter#1{\def\@currentchapter{#1}}%

%Chapter bilgilerini düzenlemek için kullanılan yani komut tanımı
\renewcommand{\chapter}{\if@openright\cleardoublepage\else\clearpage\fi%
						\thispagestyle{plain}%
						\global\@topnum\z@%
						\@afterindentfalse%
						\secdef\@chapter\@schapter}%
%
\def\@chapter[#1]#2{\ifnum\c@secnumdepth >\m@ne%
    				\refstepcounter{chapter}%
					\typeout{\@chapapp\space\thechapter.}%
					\addcontentsline{toc}{chapter}%
					{\protect\numberline{\thechapter}#1}%
					\else%
        			\addcontentsline{toc}{chapter}{#1}%
        			\fi%
                    \chaptermark{#1}%
                    \addtocontents{lof}{\protect\addvspace{10\p@}}%
                    \addtocontents{lot}{\protect\addvspace{10\p@}}%
                    \@makechapterhead{#2}%
                    \@afterheading}%
%
\def\@schapter#1{\@makeschapterhead{#1}%
                 \@afterheading}%
%
\def\@makechapterhead#1{\getchapter{#1}%
						\vspace*{50\p@}%
						{\parindent \z@ \raggedright \normalfont%
			 				\ifnum \c@secnumdepth >\m@ne%
			    			\Large\bf \space \thechapter . % Chapter number
			 				\fi%
			 			\Large #1\par% chapter title
			 			\vspace{5pt}% add vertical space
}}%
%
\def\@makeschapterhead#1{\vspace*{5\p@}%
							{\parindent\z@\raggedright%
			 				\normalfont%
			 				\interlinepenalty\@M %
			 				\Huge #1\par% chapter title
			 				\vspace{5pt}% add vertical space
}}%

%Arkaplanın kapak sayfalarında ve içindekiler sayfasında kullanılmaması için gerekli olan kodlar.
\newcommand*\SetBgContents[1]{%
  \def\bg@contents{#1}}
\newcommand*\SetBgColor[1]{%
  \def\bg@color{#1}}
\newcommand*\SetBgAngle[1]{%
  \def\bg@angle{#1}}
\newcommand*\SetBgOpacity[1]{%
  \def\bg@opacity{#1}}
\newcommand*\SetBgScale[1]{%
  \def\bg@scale{#1}}
\newcommand*\SetBgPosition[1]{%
  \def\bg@position{#1}}
\newcommand*\SetBgAnchor[1]{%
  \def\bg@anchor{#1}}
\newcommand*\SetBgHshift[1]{%
  \def\bg@hshift{#1}}
\newcommand*\SetBgVshift[1]{%
  \def\bg@vshift{#1}}

\newcommand\bg@material{%
  \begin{tikzpicture}[remember picture,overlay]
  \node [rotate=\bg@angle,scale=\bg@scale,opacity=\bg@opacity,%
    xshift=\bg@hshift,yshift=\bg@vshift,color=\bg@color]
    at (\bg@position) [\bg@anchor]  {\bg@contents};
  \end{tikzpicture}}%
\newcommand\BgThispage{\AddThispageHook{\bg@material}}

\newcommand\NoBgThispage{%
  \let\oldbg@material\bg@material\renewcommand\bg@material{}%
  \afterpage{\AddEverypageHook{\oldbg@material}}}%

\ifbg@some%
  \AddThispageHook{}%
\else%
  \AddEverypageHook{\bg@material}
\fi

%Dökmanda kullanlan uzunluk tanımları
\newlength{\picturewidth}%
\setlength{\picturewidth}{15em}%Resim genişliğini belirler

%Sayfaya gerekli olan antenti atar. Bu antent içeriği DEU logosu, DEU MUH FAK logosu ve tam ortasına gelecek şekilde yazılardır.
\newcommand{\antet}{
  \vspace*{-1em}%
  \hspace{-0.45\picturewidth}\includegraphics[width=0.5\picturewidth]{Deu.jpg}%Sayfanın sol tarafına DEU logosu basar.
  \parbox[b][][t]{0.8\textwidth}{\centering%
    \textbf{\large T.C. \\ DOKUZ EYLÜL UNIVERSITY \\ ENGINEERING FACULTY \\ ELECTRICAL \& ELECTRONICS ENGINEERING \\ DEPARTMENT}}%Sayfanın ortasına %okulun künyesini yazar.
  \includegraphics[width=0.5\picturewidth]{Muh.jpg}%Sayfanın sağ tarafına DEU Muh Fak logosu yerleştirir.
  \vfill%
}%

%Bu komut sayfanın arkasına sadece filigran basar
\newcommand{\fligran}{\SetBgContents{%
  \begin{tikzpicture}[remember picture,overlay,opacity=0.1]%
    \draw[white] (current page.north west) --node[midway] (merkez) {} (current page.south east);%Sayfanın tam ortasını bulur.
    \node at (merkez) {\includegraphics[width=10cm]{\@logofile}};%
  \end{tikzpicture}%
}}%

%Bu komut tanımı form sayfasını oluşturur.
\newcommand{\form}{%
	 \thispagestyle{empty}
 	\fligran%
 	{\Large
 	\begin{center}
 	\textbf{THESIS EVALUATION FORM}
 	\end{center}
	We certify that we have read this thesis and that in our opinion it is fully adequate, in scope and qualify as an undergraduate thesis, based on the result of the oral examination taken place on \rule{3ex}{1pt}/\rule{3ex}{1pt}/\rule{9ex}{1pt} \\
	\vfill
	\begin{center}
		\Large
		\@advisor \\%
		(ADVISOR)
		\vfill
		\begin{tabular}{c c}%
			\Large
			{\@jurya} & {\@juryb} \\
			(COMMITTEE MEMBER) & (COMMITTEE MEMBER) \\
  		\end{tabular}
  		\vfill
  		{\@chair} \\
		(CHAIRPERSON) \\
  		\vfill
  	\end{center}}
}%
  
\newcommand{\tmpsection}[1]{}%
\let\tmpsection=\section%
\renewcommand{\section}[1]{\getsection{#1}\tmpsection{#1}}%


%Sayfa numaralarının tekar arabic fonta dönmesini sağlar ve döküman yazılmaya başlanırken mutlaka yazılmalıdır.
\newcommand{\start}{\setcounter{page}{1}\renewcommand{\thepage}{\arabic{page}}}

%içindekiler komutunu yeniden tanımlar.
\renewcommand\tableofcontents{%sayfanın arkansına staj yapılan kurumun logosunu yerleştirir.
  \SetBgContents{
  \begin{tikzpicture}[remember picture,overlay,opacity=0.1]%
    \draw[white] (current page.north west) --node[midway] (merkez) {} (current page.south east);%Sayfanın tam ortasını bulur.
    \node at (merkez) {\includegraphics[width=10cm]{\@logofile}};%
  \end{tikzpicture}%
}
\chapter*{\contentsname}%
   \addtocounter{page}{0}%
   \addcontentsline{toc}{chapter}{\contentsname}%
   \@starttoc{toc}%
   \ifodd\value{page}{\cleardoublepage}\else{\clearpage}\fi%
}%
%

%Şekil listesini düzenleyen komutu yeniden düzenler.
\renewcommand\listoffigures{%sayfanın arkasına staj yapılan kurumun logosunu yerleştirir.
\fligran%
\chapter*{\listfigurename}
   \addcontentsline{toc}{chapter}{\listfigurename}%
  \@starttoc{lof}%
  \ifodd\value{page}{\cleardoublepage}\else{\clearpage}\fi%
}%
%
%Tablo listesini yeniden tannımlar 
\renewcommand\listoftables{%sayfanın arkasına staj yapılan kurumun logosunu yerleştirir.
 \fligran%
 \chapter*{\listtablename}%
   \addcontentsline{toc}{chapter}{\listtablename}%
   \@starttoc{lot}%
   \ifodd\value{page}{\cleardoublepage}\else{\clearpage}\fi%
}

%Bu komut giriş sayfalarının güz ve bahar dönemine göre düzenlenmesini sağlar.
\renewcommand{\titlepage}{%
\if@thesis%
  \titlepageB%
\else%
  \titlepageA%
\fi%

}%

\newcommand{\titlepageA}{%   
  \vfill
  \antet%Anteti sayfanın başına yerleştirir.   
   \vspace*{7em}
  \begin{center}%Kapakta gerekli oaln yazıları yazar ve düzenlemeleri yapar.
  	\setlength{\baselineskip}{1.5\baselineskip}{
    { \textbf{\LARGE  \@projectname}}\par% Projenin adını yazar
      \vspace*{7em}
    {\textbf{\Large{Research Project Report}}}\par% Tezin dönemini yazar
     \vspace*{3.5em}
    {\textit{\Large by}}\par%
      \vspace*{0.7em}
    {\textbf{\Large{\@ogrencininadi}}}\par%Öğrencinin adını yazar
     \vspace*{3.5em}
    {\textit{\Large{Advisor}}}\par% 
      \vspace*{0.7em}
    {\textbf{\Large{\@advisor}}}\par}% Danışmanın adını yazar
    \end{center}
    \vspace*{3.5em}
   \begin{center}
    \setlength{\baselineskip}{1\baselineskip}{
    {\large{\@time}}\par%Defterin yazıldığı tarihi yazar
    {\large IZMIR}\par}%Okulun bulunduğu şehiri yazar.
  \end{center}%
  \newpage%
  \expandafter \fligran
  \setlength{\baselineskip}{1.5\baselineskip}
}%

\newcommand{\titlepageB}{%   
  \thispagestyle{empty}%
  \vfill
  \antet%Anteti sayfanın başına yerleştirir.   
   \vspace*{7em}
  \begin{center}%Kapakta gerekli oaln yazıları yazar ve düzenlemeleri yapar.
  	\setlength{\baselineskip}{1.5\baselineskip}{
    { \textbf{\LARGE  \@projectname}}\par% Projenin adını yazar
      \vspace*{7em}
    {\textbf{\Large{Final Project}}}\par% Tezin dönemini yazar
     \vspace*{3.5em}
    {\textit{\Large by}}\par%
      \vspace*{0.7em}
    {\textbf{\Large{\@ogrencininadi}}}\par%Öğrencinin adını yazar
     \vspace*{3.5em}
    {\textit{\Large{Advisor}}}\par% 
      \vspace*{0.7em}
    {\textbf{\Large{\@advisor}}}\par}% Danışmanın adını yazar
    \end{center}
    \vspace*{3.5em}
   \begin{center}
   	\setlength{\baselineskip}{1\baselineskip}{
    {\large{\@time}}\par%Defterin yazıldığı tarihi yazar
    {\large IZMIR}\par}%Okulun bulunduğu şehiri yazar.
  \end{center}%
  \newpage%
  \thispagestyle{empty}%
  \setlength{\baselineskip}{1.5\baselineskip}
  \form%Form sayfasını yerleştirir.
  \setcounter{page}{0}
  \newpage%
  \expandafter \fligran
}%

%Sayfa numarasını ayarlar
\setcounter{page}{0}%Sayfa sayacını sıfırlar
  \renewcommand{\thepage}{\Roman{page}}%Sayfa nımaralrının stilini arabicden romana çevirir.
  
% Acknowledgements ortamını düzenler  
\newenvironment{acknowledgements}{\begin{center}\textbf{\Large ACKNOWLEDGEMENTS}\end{center}}{\addcontentsline{toc}{chapter}{ACKNOWLEDGEMENTS}}

% Abstract ortamını düzenler  
\renewenvironment{abstract}{\newpage \begin{center}\textbf{\Large ABSTRACT}\end{center}}{\addcontentsline{toc}{chapter}{ABSTRACT}}

% Özet ortamını düzenler  
\newenvironment{ozet}{\newpage \begin{center}\textbf{\Large ÖZET}\end{center}}{\addcontentsline{toc}{chapter}{ÖZET}}


\AtBeginDocument{\titlepage}%Dökümanın her şartta başlangıç sayfaları ile birlikte dizilmesini sağlar.

%Döküman sınıfı içinde kullanılan komutları ilk değer ataması yapar.
\watermarklogo{Deu.jpg}  % Dokuz Eylül Üniversitesi Logosu; Sayfaya arkasına varsayılan logo olarak basılır.
\projectname{~}% Proje adı
\ogrencininadi{~}% Öğrenci adı
\advisor{~}% Danışman
\time{~}% Raporlama zamanı
\jurya{~}% Jüri üyesi
\juryb{~}% Jüri üyesi
\chair{~}% Bölüm başkanı
\getsection{~}%
\getchapter{~}%
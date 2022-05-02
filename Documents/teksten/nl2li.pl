#!/usr/bin/perl
# 
# (c) 2000 Robert Brady
#     2002 Bastien Nocera
#     2003 Kenneth Christiansen
# 
# Released under the GNU General Public Licence, either version 2
# or at your option, any later version
#
# Script to take a nl.po file and make an Limburgish translation of it.
# since full AI hasn't been invented yet, you need to inspect it
# by hand afterwards.
 
use Time::gmtime;

my $author = "Mathieu van Woerkom <mathieu.vanwoerkom@hotmail.com>";
my $start = 1; 

# about about (,),:,_ in front of or after word? Speed considerations?

sub letter_comp
{
    my ($tf, $tt) = @_;

    $msg_str =~ s/$tf/$tt/g;
    $msg_str =~ s/\u$tf/\u$tt/g;
    $msg_str =~ s/\U$tf/\U$tt/g;
}

sub single_word
{
    my ($tf, $tt) = @_;

    # print STDERR $msg_str;

    $msg_str =~ s/ $tf([\.\:])? / $tt$1 /g;
    $msg_str =~ s/ \u$tf([\.\:])? / \u$tt$1 /g;
    $msg_str =~ s/ \U$tf([\.\:])? / \U$tt$1 /g;
}

sub prefix_word
{
    my ($tf, $tt) = @_;

    $msg_str =~ s/ $tf/ $tt/g;
    $msg_str =~ s/ \u$tf/ \u$tt/g;
    $msg_str =~ s/ \U$tf/ \U$tt/g;
}

sub suffix_word
{
    my ($tf, $tt) = @_;

    $msg_str =~ s/$tf /$tt /g;
    $msg_str =~ s/\u$tf /\u$tt /g;
    $msg_str =~ s/\U$tf /\U$tt /g;
}
 
sub translate() 
{
   if ($start == 1 && $msg_str =~ /Project-Id-Version/) {

     $date = sprintf("%04i-%02i-%02i %02i:%02i+0000", gmtime()->year+1900,
     gmtime()->mon+1, gmtime()->mday, gmtime()->hour, gmtime()->min);
   
     $msg_str =~ s/YEAR-MO-DA HO:MI\+ZONE/$date/;
     $msg_str =~ s/PO-Revision-Date:.*/PO-Revision-Date: $date\\n\"/;
     $msg_str =~ s/Last-Translator:.*/Last-Translator: $author\\n\"/;
     $msg_str =~ s/Content-Type:.*/Content-Type: text\/plain; charset=UTF-8\\n\"/;
     $msg_str =~ s/Content-Transfer.*/Content-Transfer-Encoding: 8bit\\n\"/;
     $msg_str =~ s/Language-Team:.*/Language-Team: Limburgish <li@li.org>\\n\"/;

     $start = 0;
     return;
   }
 
   # add space to begin and end
   $msg_str =~ s/^\"/\" /;
   $msg_str =~ s/\"$/ \"/;

   # single word is slow!

   single_word ("actie", "aksie");
   single_word ("aanklikken","aanklikke");
   single_word ("aankruisen","aankruutse");
   single_word ("aankruisvakje","aankruutsfak");
   single_word ("aankruisvak","aankruutsfak");
   single_word ("aankruis","aankruuts");
   single_word ("aantal","deil");
   single_word ("absoluut","abseluut");
   single_word ("absolute","abselute");
   single_word ("afbeeldingen","aafbiljinge");
   single_word ("afbeelding","aafbiljing");
   single_word ("afdruk","aafdrök");
   single_word ("afneembaar","aafnumbaar");
   single_word ("afnemen","aafnumme");
   single_word ("afspelen","aafsjpele");
   single_word ("afstand","aafsjtandj");
   single_word ("alleen","allein");
   single_word ("allocatie","allokasie");
   single_word ("als","es");
   single_word ("animatie","animasie");
   single_word ("animatie","animasie");
   single_word ("automatisch","automatis");
   single_word ("automatische","automatisse");
   single_word ("ballon","ballóng");
   single_word ("bedoelen","meine");
   single_word ("bedoelt","meint");
   single_word ("bedoeld","gemeind");
   single_word ("bedoeling","aafzich");
   single_word ("beëindigen","beènjige");
   single_word ("beëindigt","beènjig");
   single_word ("beëindigd","beènjig");
   single_word ("beëindigde","beènjigde");
   single_word ("beide","beids");
   single_word ("bellen","belle");
   single_word ("benodigde","benudigde");
   single_word ("benodigd","benudig");
   single_word ("berichten","berichte");
   single_word ("bericht","berich");
   single_word ("bestaat","besjteit");
   single_word ("bestand","besjtandj");
   single_word ("bestanden","besjtenj");
   single_word ("bevatten","bevatte");
   single_word ("bevindt","bevundj");
   single_word ("bewaard","verwaard");
   single_word ("bewaart","verweurt");
   single_word ("bewaren","verware");
   single_word ("bewerken","bewirke");
   single_word ("bij","bie");
   single_word ("binnekomen","inkómme");
   single_word ("binnenkomt","inkump");
   single_word ("binnen","bènne");
   single_word ("blad","blaad");
   single_word ("boom","boum");
   single_word ("breedte","breide");
   single_word ("bureau","bero");
   single_word ("bureaublad","beroblaad");
   single_word ("canvas","kanvas");
   single_word ("cel","sel");
   single_word ("code","kood");
   single_word ("constateren","vassjtèlle");
   single_word ("constateerd","sjtèlt vas");
   single_word ("controle","kentrol");
   single_word ("creëren","make");
   single_word ("creëert","maak");
   single_word ("creëerde","maakde");
   single_word ("datum","datem");
   single_word ("detail","detaj");
   single_word ("details","detajs");
   single_word ("document","dokkemènt");
   single_word ("druk","drök");
   single_word ("drukken","drökke");
   single_word ("drukknop","drökknóp");
   single_word ("een","ein");
   single_word ("effect","effek");
   single_word ("eigenschap","eigesjap");
   single_word ("eigenschappen","eigesjappe");
   single_word ("eindelijk","èntelings");
   single_word ("eindigen","ènjige");
   single_word ("eindigt","ènjig");
   single_word ("eindigde","ènjigde");
   single_word ("eind","ènj");
   single_word ("fout","faeler");
   single_word ("gebied","gebeed");
   single_word ("gebied","gebeed");
   single_word ("gebruik","gebroek");
   single_word ("gebruiker","gebroeker");
   single_word ("gebruikersnaam","gebroekersnaam");
   single_word ("geconstateerd","vasgesjtèld");
   single_word ("gecreëerd","gemaak");
   single_word ("gedeactiveerd","gedekativeerd");
   single_word ("gedeeld","gedeild");
   single_word ("gedeeltelijk","gedeiltelik");
   single_word ("gedelete","gedeilte");
   single_word ("geen","gein");
   single_word ("geëindigd","geènjig");
   single_word ("gehele","ganse");
   single_word ("gegevens","gegaeves");
   single_word ("gegeven","gagaeve");
   single_word ("geklikt","geklik");
   single_word ("genoemd","geneump");
   single_word ("gereedschap","getuug");
   single_word ("gestuurd","gesjik");
   single_word ("gewenste","gewunsjde");
   single_word ("glas","glaas");
   single_word ("grootte","gruutde");
   single_word ("heb","höb");
   single_word ("hebben","höbbe");
   single_word ("hebt","höbt");
   single_word ("heeft","haet");
   single_word ("het","'t");
   single_word ("hier","hie");
   single_word ("hoofd","huid");
   single_word ("hoogte","huugde");
   single_word ("huidige","hujige");
   single_word ("ieder","jeder");
   single_word ("iedere","jeder");
   single_word ("indien","wen");
   single_word ("ingeschakeld","ingesjakeld");
   single_word ("ingesteld","ingesjtèld");
   single_word ("inhoud","inhaud");
   single_word ("initialiseren","inisialisere");
   single_word ("instellen","insjtèlle");
   single_word ("invoerveld","inveurveldj");
   single_word ("invoeren","inveure");
   single_word ("invoer","inveur");
   single_word ("kalender","kelender");
   single_word ("kanaal","kenaal");
   single_word ("kan","kèn");
   single_word ("kantelen","kwinkele");
   single_word ("keuzevak","keusfak");
   single_word ("kies","keus");
   single_word ("klikken","klikke");
   single_word ("knop","knóp");
   single_word ("knop","knóp");
   single_word ("knoppen","knóppe");
   single_word ("kolom","kelóm");
   single_word ("kon","kós");
   single_word ("kunnen","kènne");
   single_word ("laatst","lèst");
   single_word ("laatste","lèste");
   single_word ("laden","laje");
   single_word ("leeg","laeg");
   single_word ("lees","laes");
   single_word ("lengte","lengde");
   single_word ("lege","laeg");
   single_word ("lezen","laeze");
   single_word ("lijst","lies");
   single_word ("maar","mer");
   single_word ("machine","mesjien");
   single_word ("mailbox","postbös");
   single_word ("mag","maog");
   single_word ("maken","make");
   single_word ("mappen","mappe");
   single_word ("mee","mit");
   single_word ("menuobject","menuobjek");
   single_word ("met","mit");
   single_word ("minuten","minute");
   single_word ("mislukken","mislökke");
   single_word ("mislukte","mislökde");
   single_word ("mislukt","mislök");
   single_word ("misschien","mesjiens");
   single_word ("moet","moot");
   single_word ("moeten","mote");
   single_word ("mogelijk","mäögelik");
   single_word ("naar","nao");
   single_word ("na","nao");
   single_word ("niet","neet");
   single_word ("niets","niks");
   single_word ("nieuw","nuuj");
   single_word ("nieuwe","nuuj");
   single_word ("nul","nöl");
   single_word ("object","objek");
   single_word ("omdat","wiel");
   single_word ("omhoog","ómhoeg");
   single_word ("omlaag","ómlieg");
   single_word ("omschrijving","ómsjrieving");
   single_word ("onbekend","ónbekènd");
   single_word ("onder","ónger");
   single_word ("ondersteunen","sjtiepe");
   single_word ("ondersteuning","sjtiep");
   single_word ("ondersteund","gesjtiep");
   single_word ("ongeldige","óngeljige");
   single_word ("ongeldig","óngeljig");
   single_word ("ongelezen","óngelaeze");
   single_word ("onvolledige","ónvolsjtendige");
   single_word ("onvolledig","ónvolsjtendig");
   single_word ("opdracht","opdrach");
   single_word ("openen","äöpene");
   single_word ("open","äöpen");
   single_word ("operatie","operasie");
   single_word ("opslaan","opsjlaon");
   single_word ("optie","opsie");
   single_word ("opties","opsies");
   single_word ("opvragen","opvraoge");
   single_word ("oriëntatie","orjentasie");
   single_word ("orn","órn");
   single_word ("oud","aud");
   single_word ("over","euver");
   single_word ("pad","paad");
   single_word ("paneel","peniel");
   single_word ("paneel-object","penielobjek");
   single_word ("paragraaf","parregraaf");
   single_word ("pictogram","piktogram");
   single_word ("pijl","piel");
   single_word ("plek","plèk");
   single_word ("positie","posisie");
   single_word ("postbus","postbös");
   single_word ("probleem","prebleem");
   single_word ("programma","program");
   single_word ("reeks","riej");
   single_word ("restant","euversjot");
   single_word ("rij","riej");
   single_word ("schaduw","sjeem");
   single_word ("schakel","sjakel");
   single_word ("scheiding","sjeit");
   single_word ("schema","sjema");
   single_word ("scherm","sjirm");
   single_word ("schrijf","sjrief");
   single_word ("schuif","sjuuf");
   single_word ("schuiven","sjuve");
   single_word ("schuiver","sjuver");
   single_word ("seconde","sekónd");
   single_word ("secondes","sekónde");
   single_word ("selecteer","keus");
   single_word ("slaan","sjloan");
   single_word ("sneltoets","sjnaktósj");
   single_word ("soort","saort");
   single_word ("staat","sjtaot");
   single_word ("standaard","sjtanderd");
   single_word ("starten","stjarte");
   single_word ("steunen","sjtiepe");
   single_word ("steun","sjtiep");
   single_word ("sturen","sjikke");
   single_word ("synchroon","sinkroon");
   single_word ("tekens","teikes");
   single_word ("teken","teike");
   single_word ("tekst","teks");
   single_word ("terug","trök");
   single_word ("tijdens","tiedes");
   single_word ("tijd","tied");
   single_word ("titel","tittel");
   single_word ("toets","tósj");
   single_word ("toetsen","tósje");
   single_word ("tonen","tuine");
   single_word ("type","tiep");
   single_word ("u","geer");
   single_word ("u heeft","geer höb");
   single_word ("uit","oet");
   single_word ("uiterlijk","uterlik");
   single_word ("uitvoeren","oetveure");
   single_word ("uw","eur");
   single_word ("vallen","valle");
   single_word ("vanaf","vanaaf");
   single_word ("veld","veldj");
   single_word ("venster","vinster");
   single_word ("verandering","verangering");
   single_word ("verbergen","maskere");
   single_word ("verbergknoppen","maskeringsknóppe");
   single_word ("verbinden","verbènje");
   single_word ("verder","wiejer");
   single_word ("vergrendelen","vaszètte");
   single_word ("verkeerd","verkierd");
   single_word ("verkeerde","verkierde");
   single_word ("verversen","verfrisje");
   single_word ("verversing","verfrisjing");
   single_word ("ververst","verfrisj");
   single_word ("verwijderen","ewegdoon");
   single_word ("verzameling","zameling");
   single_word ("vinden","vènje");
   single_word ("vis","vèsj");
   single_word ("voet","voot");
   single_word ("volledige","volsjtendige");
   single_word ("volledig","volsjtendig");
   single_word ("voor","veur");
   single_word ("voordat","ierdet");
   single_word ("voorkeur","prifferensie");
   single_word ("voortgang","avvans");
   single_word ("vuller","völler");
   single_word ("waarschijnlijk","dènkelik");
   single_word ("waarschuwt","waarsjoewt");
   single_word ("wachtwoord","wachwaord");
   single_word ("wachtwoord","wachwaord");
   single_word ("wachtwoorden","wachwäörd");
   single_word ("wanneer","wen");
   single_word ("was","waor");
   single_word ("werd","woor");
   single_word ("werk","wirk");
   single_word ("wij","veer");
   single_word ("woord","waord");
   single_word ("woorden","wäörd");
   single_word ("word","waer");
   single_word ("worden","waere");
   single_word ("wordt","weurt");
   single_word ("zetten","zètte");
   single_word ("zicht","zich");
   single_word ("zichtbaar","zichbaar");
   single_word ("zien","zeen");
   single_word ("zijn","zeen");
   single_word ("zodat","zoedet");
   single_word ("zou","zouw");
   single_word ("zoeken","zeuke");
   single_word ("zoek","zeuk");
   single_word ("zullen","zólle");
   single_word ("zwevend","sjwevend");
   single_word ("zweven","sjweve");

   letter_comp ("programma", "program");
   letter_comp ("gebruiker", "gebroeker");
   letter_comp ("controle", "kentrol");
   letter_comp ("animatie", "animasie");
   letter_comp ("tijd", "tied");
   letter_comp ("paneel", "peniel");
   letter_comp ("afbeelding", "aafbiljing");
   letter_comp ("tekst", "teks");
   letter_comp ("veld", "veldj");
   letter_comp ("venster", "vinster");
   letter_comp ("scherm", "sjirm");

   suffix_word ("uit", "oet");   # ex: uitvoeren
   suffix_word ("niet", "neet"); # ex: niet-bestaande lokaties

   prefix_word ("type", "tiep");
   prefix_word ("object", "objek");
   prefix_word ("knop", "knóp");
   prefix_word ("toets", "tósj");
   prefix_word ("toetsen", "tósje");

   single_word ("translator_credits", $author);

   $msg_str =~ s/^\" /\"/;
   $msg_str =~ s/ \"$/\"/;
}
 
$mode = 0;

$time = gmtime()->min * 60 + gmtime()->sec;

while (<>) 
{
    if  (/^#/) 
    {
	s/SOME DESCRIPTIVE TITLE/Limburgish (Limburgs) translation/;
	$year = gmtime()->year+1900;
	s/YEAR/$year/;
	s/FIRST AUTHOR <EMAIL\@ADDRESS>/$author/;
	print unless ((/^#, fuzzy/) && ($mode eq 0));
    } 
    elsif (/^msgid /) 
    {
	$msg_id .= substr($_, 6);
	$mode = 1;
    } 
    elsif (/^msgstr "/) 
    {
        $msg_str .= substr($_, 7);
        $mode = 2;
    } 
    elsif (/^"/) 
    {
	if ($mode == 1) {
	    $msg_id .= $_;
	    $mode = 1;
        } elsif ($mode == 2) {
	    $msg_str .= $_;
	    $mode = 2;
        }
    } 
    else 
    {
	if ($msg_id || $msg_str) 
	{
	    translate();
	    print "msgid $msg_id";
	    print "msgstr $msg_str";
	    $msg_id = "";
	    $msg_str = "";
	}
	print $_;
    }
}
 
if ($msg_id || $msg_str) 
{
   translate();

   print "msgid $msg_id";
   print "msgstr $msg_str";
   $msg_id = "";
   $msg_str = "";
}

$time = (gmtime()->min * 60 + gmtime()->sec) - $time;

print STDERR "Time spend " . $time . " sec.\n";

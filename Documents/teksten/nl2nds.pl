#!/usr/bin/perl
# 
# (c) 2000 Robert Brady
#     2002 Bastien Nocera
#     2003 Kenneth Christiansen
# 
# Released under the GNU General Public Licence, either version 2
# or at your option, any later version
#
# Script to take a nl.po file and make an Lowlands Saxon translation of it.
# Since full AI hasn't been invented yet, you need to inspect it
# by hand afterwards.
 
# What is this?
# 
# I created this script since it is easier translating from a close 
# related language, that starting from scratch. When you do that, you
# end up doing a lot of work which could be automated. This script
# automates some of it.

use Time::gmtime;

my $author = "Kenneth Christiansen <kenneth@gnu.org>";
my $team   = "Lowlands Saxon <nds@li.org>";
my $desc   = "North Lowlands Saxon (New Hansatic Orthography) translation";

my $start = 1; 

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

    $msg_str =~ s/^$tf /$tt /;
    $msg_str =~ s/ $tf / $tt /g;
	
    $msg_str =~ s/^\"\u$tf /\"\u$tt /;
    $msg_str =~ s/ \u$tf / \u$tt /g;

    $msg_str =~ s/^\"\U$tf /\"\U$tt /;
    $msg_str =~ s/ \U$tf / \U$tt /g;
}
 
sub translate () 
{
   if ($start == 1 && $msg_str =~ /Project-Id-Version/) 
   {

       $date = sprintf("%04i-%02i-%02i %02i:%02i+0000", gmtime()->year+1900,
       gmtime()->mon+1, gmtime()->mday, gmtime()->hour, gmtime()->min);
   
       $msg_str =~ s/YEAR-MO-DA HO:MI\+ZONE/$date/;
       $msg_str =~ s/PO-Revision-Date:.*/PO-Revision-Date: $date\\n\"/;
       $msg_str =~ s/Last-Translator:.*/Last-Translator: $author\\n\"/;
       $msg_str =~ s/Content-Type:.*/Content-Type: text\/plain; charset=UTF-8\\n\"/;
       $msg_str =~ s/Content-Transfer.*/Content-Transfer-Encoding: 8bit\\n\"/;
       $msg_str =~ s/Language-Team:.*/Language-Team: $team\\n\"/;

       $start = 0;

       return;
   }

   ## Please order with most complete sentences first
   ##
   ## Like first sjirmafdrök, then sjirm.
   ##
   ## 'single_word' means that there has to be space around the
   ## word. Ie. it cannot be part of another word.

   letter_comb ("scherm", "schyrm");
   single_word ("het", "'t");
   single_word ("deze", "dizze"); 
   single_word ("met", "myt");
   single_word ("een", "eyn"); 
   single_word ("niet", "nich");
   letter_comb ("tijd", "tyd");
   single_word ("nieuw", "ney");
   single_word ("bij", "by");
   single_word ("geen", "kyn");
   single_word ("voor", "veur");
   letter_comb ("paneel", "panyl");   

   single_word ("translator_credits", $author);
}

$mode = 0;
 
while (<>) 
{
    if  (/^#/) 
    {
	s/SOME DESCRIPTIVE TITLE/$desc/;
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
        if ($mode == 1) 
        {
            $msg_id .= $_;
            $mode = 1;
        } 
        elsif ($mode == 2) 
        {
            $msg_str .= $_;
            $mode = 2;
        }
    } 
    else 
    {
        if ($msg_id || $msg_str) 
        {
            translate ();

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
    translate ();

    print "msgid $msg_id";
    print "msgstr $msg_str";

    $msg_id = "";
    $msg_str = "";
}

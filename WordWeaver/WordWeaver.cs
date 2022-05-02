using System;
using System.Collections;
using System.Drawing;
using Gtk;
using GtkSharp;
using GLib;
using System.Xml;

namespace Bosk {

    public enum ElementType
    {
	Word = 0,

	Reference,
	Plural,
	Conjugation,
	Gender,

	WordType,
	Pronounciation,
	Etymology,
	Description,
	Example
    }

    public class DictionaryEditor : Gtk.Window
    {
	private string app_name = "Bosk WordWeaver";

	private VBox vbox;
	private MenuBar mbar;
	private HPaned pane;
	private Statusbar statusbar;

	private MenuItem add_to_menuitem;

	private HButtonBox buttonbox;
	private Button add_btn;
	private Button edit_btn;

        private VBox right_vbox;
	private EntryView entry_view;

	private TreeView entry_tree;
	private EntryStore entry_store;
	private TreeIter active_entry = TreeIter.Zero;

	private string filename = "";
	private string dict_title;

	public string DictTitle
	{
	    get { return dict_title; }
	    set { 
		dict_title = value; 
		Title = app_name + " - " + dict_title;
	    }
	}

	public static int Main (string [] args)
	{
	    Application.Init ();

	    DictionaryEditor editor = new DictionaryEditor (args);

	    Application.Run ();

	    return 0;
	}

	public DictionaryEditor (string [] args) : base ("Bosk WordWeaver")
	{
	    int width = 650;
	    int height = 500;

	    DefaultSize = new System.Drawing.Size (width, height);
	    DeleteEvent += new DeleteEventHandler (Window_Delete);

	    vbox = new VBox (false, 4);
	    Add (vbox);
	   
	    mbar = new MenuBar ();
	    BuildMenuBar (mbar);
	    pane = new HPaned ();
	    pane.Position = width - 250;
	    statusbar = new Statusbar ();
	    vbox.PackStart (mbar, false, false, 0);
	    vbox.PackStart (pane);
	    vbox.PackStart (statusbar, false, false, 0);

	    ScrolledWindow swin = new ScrolledWindow ();
	    swin.Visible = true;
	    swin.CanFocus = true;
	    swin.HscrollbarPolicy = PolicyType.Automatic;
	    swin.VscrollbarPolicy = PolicyType.Always;
	    swin.ShadowType = ShadowType.None;

	    entry_tree = new TreeView ();
	    entry_tree.Visible = true;
	    entry_tree.CanFocus = true;
	    entry_tree.Reorderable = false;
	    entry_tree.EnableSearch = true;

	    swin.AddWithViewport (entry_tree);

	    add_to_menuitem.Sensitive = false;

	    entry_view = new EntryView ();
	    right_vbox = new VBox (false, 4);

	    buttonbox = new HButtonBox ();
	    buttonbox.LayoutStyle = ButtonBoxStyle.End;
	    buttonbox.Homogeneous = true;
	    buttonbox.BorderWidth = 8;
	    buttonbox.Spacing = 8;
	    add_btn = new Button (Stock.Add);
	    add_btn.Label = "_Add entry";
	    add_btn.Clicked += new EventHandler (AddButton_Clicked);
	    edit_btn = new Button (Stock.Add);
	    edit_btn.Label = "_Edit entry";
	    edit_btn.Clicked += new EventHandler (EditButton_Clicked);
	    edit_btn.Sensitive = false;
	    buttonbox.PackStart (add_btn, false, false, 4);
	    buttonbox.PackStart (edit_btn, false, false, 4);

	    right_vbox.PackStart (entry_view);
	    right_vbox.PackStart (buttonbox, false, false, 0);

	    entry_store = new EntryStore ();
	 
	    entry_tree.Model = entry_store;
	    entry_tree.HeadersVisible = true;
	    entry_tree.WidthRequest = 300;

	    entry_tree.RowActivated += new RowActivatedHandler (EntryView_RowActivated);
	    //entry_store.RowChanged += RowChangedHandler (WordStore_RowActivated);

	    CellRendererText cell_renderer = new CellRendererText ();
	    cell_renderer.IsExpander = true;

	    entry_tree.AppendColumn ("Dictionary Entries", cell_renderer, "text", 1);
	    entry_tree.AppendColumn ("Description", new CellRendererText (), "text", 2);

	    pane.Add1 (swin);
	    pane.Add2 (right_vbox);

	    PopulateEntryStore ();

	    ShowAll ();
	}

	private void BuildMenuBar (MenuBar mbar)
	{
	    // file

	    MenuItem file_item = new MenuItem ("_File");
	    Menu file_menu = new Menu ();
	    file_item.Submenu = file_menu;

	    MenuItem new_item = new ImageMenuItem (Stock.New, new AccelGroup ());
	    new_item.Activated += new EventHandler (Menu_NewActivated);
	    file_menu.Append (new_item);
	    
	    MenuItem open_item = new ImageMenuItem (Stock.Open, new AccelGroup ());
	    open_item.Activated += new EventHandler (Menu_OpenActivated);
	    file_menu.Append (open_item);

	    ImageMenuItem addto_item = new ImageMenuItem ("Add _To");
	    addto_item.Image = new Gtk.Image (Stock.Add, IconSize.Menu);
	    addto_item.Activated += new EventHandler (Menu_AddToActivated);
	    add_to_menuitem = addto_item;
	    file_menu.Append (addto_item);

	    MenuItem save_item = new ImageMenuItem (Stock.Save, new AccelGroup ());
	    save_item.Activated += new EventHandler (Menu_SaveActivated);
	    file_menu.Append (save_item);

	    MenuItem saveas_item = new ImageMenuItem (Stock.SaveAs, new AccelGroup ());
	    saveas_item.Activated += new EventHandler (Menu_SaveAsActivated);
	    file_menu.Append (saveas_item);

	    file_menu.Append (new SeparatorMenuItem());

	    MenuItem quit_item = new ImageMenuItem (Stock.Quit, new AccelGroup ());
	    quit_item.Activated += new EventHandler (Menu_QuitActivated);
	    file_menu.Append (quit_item);

	    mbar.Append (file_item);

	    // edit
	    
	    MenuItem edit_item = new MenuItem ("_Edit");
	    Menu edit_menu = new Menu ();
	    edit_item.Submenu = edit_menu;

	    MenuItem cut_item = new ImageMenuItem (Stock.Cut, new AccelGroup ());
	    cut_item.Activated += new EventHandler (Menu_CutActivated);
	    cut_item.Sensitive = false;
	    edit_menu.Append (cut_item);

	    MenuItem copy_item = new ImageMenuItem (Stock.Copy, new AccelGroup ());
	    copy_item.Activated += new EventHandler (Menu_CopyActivated);
	    copy_item.Sensitive = false;
	    edit_menu.Append (copy_item);

	    MenuItem paste_item = new ImageMenuItem (Stock.Paste, new AccelGroup ());
	    paste_item.Activated += new EventHandler (Menu_PasteActivated);
	    paste_item.Sensitive = false;
	    edit_menu.Append (paste_item);

	    MenuItem delete_item = new ImageMenuItem (Stock.Delete, new AccelGroup ());
	    delete_item.Activated += new EventHandler (Menu_DeleteActivated);
	    delete_item.Sensitive = false;
	    edit_menu.Append (delete_item);
	   
	    mbar.Append (edit_item);
	    
	    // help
	    
	    MenuItem help_item = new MenuItem ("_Help");
	    Menu help_menu = new Menu ();
	    help_item.Submenu = help_menu;
	   
	    MenuItem about_item = new ImageMenuItem ("_About...");
	    about_item.Activated += new EventHandler (Menu_AboutActivated);
	    help_menu.Append (about_item);

	    mbar.Append (help_item);
	    
	}

	private void ReadDictionary (IO.Stream stream, bool overwrite)
	{
	    XmlTextReader reader = new XmlTextReader (stream);
	    
	    TreeIter iter = new TreeIter ();

	    if (overwrite) {
		entry_store.Clear ();
		entry_view.Clear ();
	    }

	    while (reader.Read ()) {

		if (reader.Depth == 0) {
		    if (reader.NodeType == XmlNodeType.Element &&
			reader.Name.Equals ("dictionary")) {
			if (overwrite)
			    DictTitle = reader.GetAttribute ("title");
		    } 
		}

		if (reader.Depth == 1) {
		    if (reader.NodeType == XmlNodeType.Element &&
			reader.Name.Equals ("word")) {
			iter = entry_store.AppendElement (TreeIter.Zero, 
							  ElementType.Word, 
							  reader.GetAttribute ("value"));
		    } 
		}

		if (reader.Depth == 2) {
		    if (reader.NodeType == XmlNodeType.Element) {
			switch (reader.Name) {
			case "reference":
			    entry_store.AppendElement (iter, ElementType.Reference, 
						       reader.ReadString());
			    break;
 			case "conjugation":
			    entry_store.AppendElement (iter, ElementType.Conjugation, 
						       reader.ReadString());
			    break;
			case "plural":
			    entry_store.AppendElement (iter, ElementType.Plural, 
						       reader.ReadString());
			    break;
			case "gender":
			    entry_store.AppendElement (iter, ElementType.Gender,
						       reader.ReadString());
			    break;
			case "word-type":
			    entry_store.AppendElement (iter, ElementType.WordType,
						       reader.ReadString());
			    break;
			case "pronounciation":
			    entry_store.AppendElement (iter, ElementType.Pronounciation,  
						       reader.ReadString());
			    break;
			case "etymology":
			    entry_store.AppendElement (iter, ElementType.Etymology,
						       reader.ReadString());
			    break;
			case "description":
			    entry_store.AppendElement (iter, ElementType.Description, 
						       reader.ReadString());
			    break;
			case "example":
			    entry_store.AppendElement (iter, ElementType.Example,
						       reader.ReadString());
			    break;
			    
			default:
			    break;
			}
		    }
		}
		
		add_to_menuitem.Sensitive = true;
	    }
	    
	    //	    entry_store.SetSortColumnId (1, SortType.Ascending);
	    entry_store.SetSortColumnId (3, SortType.Ascending);
	}

	private void WriteDictionary (IO.Stream stream)
	{

	    System.Text.Encoding encoding =  new System.Text.UTF8Encoding ();
	    XmlTextWriter writer = new XmlTextWriter (stream, encoding);
	    
	    writer.Formatting = Formatting.Indented;

	    writer.WriteStartDocument ();
	    writer.WriteStartElement ("dictionary");

	    writer.WriteStartAttribute ("", "title", "");
	    writer.WriteString (dict_title);
	    writer.WriteEndAttribute ();

	    TreeIter iter, child;
	    entry_store.GetIterFirst (out iter);
	    int num_nodes = entry_store.IterNChildren();

	    for (int i = 0; i < num_nodes; i++) {

		writer.WriteStartElement ("word");

		int num_children = entry_store.IterNChildren (iter);
		entry_store.IterChildren (out child, iter);

		writer.WriteStartAttribute("", "value", "");
		writer.WriteString ((string) entry_store.GetValue (iter, 1));
		writer.WriteEndAttribute ();

		for (int j = 0; j < num_children; j++) {

		    ElementType elemtype = (ElementType) entry_store.GetValue (child, 0);

		    switch (elemtype) {
		    case ElementType.Reference:
			writer.WriteStartElement ("reference");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Conjugation:
			writer.WriteStartElement ("conjugation");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Plural:
			writer.WriteStartElement ("plural");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Gender:
			writer.WriteStartElement ("gender");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.WordType:
			writer.WriteStartElement ("word-type");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Pronounciation:
			writer.WriteStartElement ("pronounciation");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Etymology:
			writer.WriteStartElement ("etymology");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Description:
			writer.WriteStartElement ("description");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
		    case ElementType.Example:
			writer.WriteStartElement ("example");
			writer.WriteString ((string) entry_store.GetValue (child, 1));
			break;
			
		    default:
			break;
		    }
		    
		    writer.WriteEndElement ();
		    entry_store.IterNext (out child);
		}
		
		writer.WriteEndElement ();
		entry_store.IterNext (out iter);
	    }

	    writer.WriteEndElement();
	   
	    writer.Close();       
	}

	private void EntryView_RowActivated (object o, RowActivatedArgs args)
	{
	    TreeStore model;
	    TreeIter iter;

	    model = entry_store;
 	    model.GetIter (out iter, args.Path);
	    //parent = args.Iter;

	    if (model.IterDepth (iter) != 0)
		return;
	    
	    edit_btn.Sensitive = true;
	    active_entry = iter;

	    entry_view.SetEntry (entry_store, iter);
	}

	private void PopulateEntryStore ()
	{
	    string filename = "data.dict";

	    IO.FileInfo finfo = new IO.FileInfo (filename);

	    if (finfo.Exists)
		ReadDictionary (System.IO.File.OpenRead (filename), true);
	}

	private static void Window_Delete (object obj, DeleteEventArgs args)
	{
	    Application.Quit ();
	}

	private void Menu_NewActivated (object o, EventArgs args)
	{
	    entry_store.Clear ();
	    entry_view.Clear ();
	    edit_btn.Sensitive = false;
	    active_entry = TreeIter.Zero;

	    filename = "";
	    DictTitle = "Untitled";
	}

	private void Menu_OpenActivated (object o, EventArgs args)
	{
	    FileSelection dialog = new FileSelection ("Open...");

	    if (dialog.Run () != (int) ResponseType.Ok) {
		dialog.Destroy ();
	    } else {	
		filename = dialog.Filename;

		IO.FileInfo finfo = new IO.FileInfo (filename);

		if (finfo.Exists) {
		    System.IO.FileStream stream = 
			System.IO.File.OpenRead (filename);
		    
		    dialog.Destroy ();
		    
		    ReadDictionary (stream, true);
		    
		    //Console.Write ("The user selected \"" + finfo.ToString () + "\"");
		}
	    }
	}

	private void Menu_AddToActivated (object o, EventArgs args)
	{
	    FileSelection dialog = new FileSelection ("Open...");

	    if (dialog.Run () != (int) ResponseType.Ok) {
		dialog.Destroy ();
	    } else {	
		filename = dialog.Filename;

		IO.FileInfo finfo = new IO.FileInfo (filename);

		if (finfo.Exists) {
		    System.IO.FileStream stream = 
			System.IO.File.OpenRead (filename);
		    
		    dialog.Destroy ();
		    
		    ReadDictionary (stream, false);
		    
		    //Console.Write ("The user selected \"" + finfo.ToString () + "\"");
		}
	    }
	}

	private void Menu_SaveActivated (object o, EventArgs args)
	{
	    if (filename.Equals ("")) {
		Menu_SaveAsActivated (o, args);
	    } else {
		System.IO.FileStream stream = System.IO.File.Create (filename);

		WriteDictionary (stream);
	    }
	}

	private void Menu_SaveAsActivated (object o, EventArgs args)
	{
	    FileSelection dialog = new FileSelection ("Save As...");

	    if (dialog.Run () != (int) ResponseType.Ok) {
		dialog.Destroy ();
	    } else {	
		filename = dialog.Filename;

		IO.FileInfo finfo = new IO.FileInfo (filename);
		System.IO.FileStream stream = System.IO.File.Create (filename);

		dialog.Destroy ();

		WriteDictionary (stream);
		
		//Console.Write ("The user selected \"" + finfo.ToString () + "\"");
	    }
	}

	private void Menu_QuitActivated (object o, EventArgs args)
	{
	    Application.Quit ();
	}

	private void Menu_CutActivated (object o, EventArgs args)
	{
	}	

	private void Menu_CopyActivated (object o, EventArgs args)
	{
	}

	private void Menu_PasteActivated (object o, EventArgs args)
	{
	}

	private void Menu_DeleteActivated (object o, EventArgs args)
	{
	}

	private void Menu_AboutActivated (object o, EventArgs args)
	{
	    AboutDialog dialog = new AboutDialog (this, 0, 1, 0); 

	    dialog.Run (); 
	    dialog.Destroy ();
	}

	private void AddButton_Clicked (object o, EventArgs args)
	{	    
	    Dialog dialog = new EditDialog (this, entry_store, TreeIter.Zero);

	    dialog.Run (); 
	    dialog.Destroy ();
	}

	private void EditButton_Clicked (object o, EventArgs args)
	{
	    Dialog dialog = new EditDialog (this, entry_store, active_entry);

	    dialog.Run (); 
	    dialog.Destroy ();
	}
    }




    public class EditDialog : Gtk.Dialog
    {
	public EditDialog (Window win, TreeStore model, TreeIter iter) 
	    : base ("New entry", win, DialogFlags.DestroyWithParent)
	{
	    Modal = true;  
	    AllowGrow = true;

	    VBox.BorderWidth = 4;

	    TreeIter child;

	    if (iter.Equals (TreeIter.Zero)) {

	    } else { 
		int num_children;
		model.IterChildren (out child, iter);
		
		num_children = model.IterNChildren (iter);
		
		Title = "Word: " + (string) model.GetValue (iter, 1);
		
		ElementType elemtype;
		
		for (int i = 0; i < num_children; i++) {
		    elemtype = (ElementType) model.GetValue (child, 0);
		
		    switch (elemtype) {
		    case ElementType.Word:
			break;
		    case ElementType.Reference:
			//   reference = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Conjugation:
			//   conjugation = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Plural:
			//   plural = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Gender:
			//   gender = (string) model.GetValue (child, 1);
			break;
		    case ElementType.WordType:
			//   wordtype = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Pronounciation:
			//  pronounciation = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Etymology:
			//  etymology = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Description:
			//  description = (string) model.GetValue (child, 1);
			break;
		    case ElementType.Example:
			//   example = (string) model.GetValue (child, 1);
			break;
			
		    default:
			break;
		    }
		    model.IterNext (out child);
		}
	    }

	    AddButton (Stock.Ok, 5);
	}
    }




    public class AboutDialog : Gtk.Dialog
    {
	public AboutDialog (Window win, int major, int minor, int build) 
	    : base ("About WordWeaver...", win, DialogFlags.DestroyWithParent)
	{
	    Modal = true;  
	    AllowGrow = false;
          
	    VBox.BorderWidth = 4;
	    VBox.Homogeneous = false;
	    VBox.Spacing = 0;

	    //	    Image image = new Image ();

	    Label title = new Label ("<big><b>Bosk WordWeaver " + 
				     major.ToString () + "." +
				     minor.ToString () + "." +
				     build.ToString () +
				     "</b></big>");
	    title.UseMarkup = true;
	    title.SetPadding (8, 8);

	    Label desc = new Label ("WordWeaver is an application for creating and " + 
				    "managing dictionaries, with the efford put on " +
				    "usability.");
	    desc.SetPadding (4, 4);
	    desc.LineWrap = true;
	    desc.Justify = Justification.Center;

	    Label copyright = new Label ("<small>(C) 2004 Kenneth Christiansen, " +
					 "Bosk Software</small>");
	    copyright.UseMarkup = true;
	    copyright.SetPadding (8, 8);

	    VBox.PackStart (title);
	    VBox.PackStart (desc);
	    VBox.PackStart (copyright);
	    VBox.ShowAll ();

	    AddButton (Stock.Ok, 5);
	}
    }




    public class EntryStore : Gtk.TreeStore
    {
	private TreeIter active_entry = TreeIter.Zero;

	public EntryStore () : base (typeof (int), typeof (string), 
				     typeof (string), typeof (string))
	{
	}

	public TreeIter AppendElement (TreeIter iter, ElementType elemtype, string str)
	{
	    string desc;

	    // hack for missing features in bindings
	    int elemnum = (int) elemtype;
	    string str2 = elemnum.ToString ();

	    switch (elemtype) {
		case ElementType.Word:
		    desc = "Word";
		    str2 = str;
		break;
		case ElementType.Reference:
		    desc = "Reference";
		break;
		case ElementType.Conjugation:
		    desc = "Conjugation";
		break;
		case ElementType.Plural:
		    desc = "Plural";
		break;
		case ElementType.Gender:
		    desc = "Gender";
		break;
		case ElementType.WordType:
		    desc = "Type of word";
		break;
		case ElementType.Pronounciation:
		    desc = "Pronounciation";
		break;
		case ElementType.Etymology:
		    desc = "Etymology";
		break;
		case ElementType.Description:
		    desc = "Description";
		break;
		case ElementType.Example:
		    desc = "Examples";
		break;
	    default:
		desc = "";
		break;
	    }
	    

	    if (iter.Equals (TreeIter.Zero))
		return AppendValues ((int) elemtype, str, desc, str2);
	    else
		return AppendValues (iter, (int) elemtype, str, desc, str2);
	}
    }




    public class EntryView : Gtk.TextView
    {
	public EntryView () : base ()
	{
	    Editable = false;
	    WrapMode = Gtk.WrapMode.Word;

	    // add the needed tags
	    TextTag bold = new TextTag ("bold");
	    bold.Weight = Pango.Weight.Bold;

	    TextTag italic = new TextTag ("italic");
	    italic.Style = Pango.Style.Italic;

	    TextTag ipa = new TextTag ("ipa");
	    ipa.Font = "Arial Unicode MS";
	    ipa.SizePoints = 10;

	    Buffer.TagTable.Add (bold);
	    Buffer.TagTable.Add (italic);
	    Buffer.TagTable.Add (ipa);
	}

	public void Clear ()
	{
	    Buffer.Clear ();
	}

	public void SetEntry (TreeStore model, TreeIter parent)
	{
	    TreeIter child;

	    string str;

	    string word = "";
	    string reference = "";
	    string conjugation = "";
	    string plural = "";
	    string gender = "";
	    string wordtype = "";
	    string pronounciation = "";
	    string etymology = "";
	    string description = "";
	    string example = "";

	    int num_children;
	    model.IterChildren (out child, parent);

	    num_children = model.IterNChildren (parent);

	    word = (string) model.GetValue (parent, 1);
	    
	    ElementType elemtype;

	    for (int i = 0; i < num_children; i++) {
		elemtype = (ElementType) model.GetValue (child, 0);
		
		switch (elemtype) {
		case ElementType.Word:
		    break;
		case ElementType.Reference:
		    reference = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Conjugation:
		    conjugation = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Plural:
		    plural = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Gender:
		    gender = (string) model.GetValue (child, 1);
		    break;
		case ElementType.WordType:
		    wordtype = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Pronounciation:
		    pronounciation = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Etymology:
		    etymology = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Description:
		    description = (string) model.GetValue (child, 1);
		    break;
		case ElementType.Example:
		    example = (string) model.GetValue (child, 1);
		    break;
		    
		default:
		    break;
		}
		model.IterNext (out child);
	    }

	    str = word + " ";

	    if (reference.Length != 0) 
		str = str + reference + " "; 

	    if (conjugation.Length != 0)
		str = str + "(conj. " + conjugation + ") "; 
	    
	    if (gender.Length != 0)
		str = str + gender + " "; 
	    
	    if (wordtype.Length != 0)
		str = str + wordtype + " ";
	    
	    if (plural.Length != 0)
		str = str + "(pl. " + plural + ") ";
	    
	    if (pronounciation.Length != 0)
		str = str + pronounciation + " ";
	    
	    if (etymology.Length != 0)
		str = str + etymology + " ";
		
	    if (description.Length != 0)
		str = str + description + " ";
	    
	    if (example.Length != 0)
		str = str + example;
	    
	    // Console.WriteLine(str.Length);
	    
	    // This doesn't work - weird
	    //	    Buffer.SetText (str);
	    
	    //	    Console.WriteLine (Buffer.Text.Length);
	    //      Console.WriteLine (Buffer.CharCount);	    

	    Buffer.Text = str;

	    TextIter start;
	    TextIter end;

	    // word
	    start = Buffer.StartIter;
	    end = Buffer.StartIter;
	    end.ForwardChars (word.Length + 1);	
	    Buffer.ApplyTag ("bold", start, end); 

	    // reference
	    if (reference.Length != 0) {
		start = end;
		end.ForwardChars (reference.Length + 1);	
		Buffer.ApplyTag ("italic", start, end); 

		return;
	    }

	    // conjugation
	    if (conjugation.Length != 0) {
		start = end;
		start.ForwardChars (1);
		end.ForwardChars (conjugation.Length + 7);
		Buffer.ApplyTag ("italic", start, end);
		end.ForwardChars (2);
	    }

	    // gender
	    if (gender.Length != 0) {
		start = end;
		end.ForwardChars (gender.Length + 1);	
		Buffer.ApplyTag ("italic", start, end); 
	    }

	    // wordtype
	    if (wordtype.Length != 0) {
		start = end;
		end.ForwardChars (wordtype.Length + 1);
	    }

	    // plural
	    if (plural.Length != 0) {
		start = end;
		start.ForwardChars (1);
		end.ForwardChars (plural.Length + 5);
		Buffer.ApplyTag ("italic", start, end);
		end.ForwardChars (2);
	    }

	    // pronounciation
	    if (pronounciation.Length != 0) {
		start = end;
		end.ForwardChars (pronounciation.Length + 1);
		Buffer.ApplyTag ("ipa", start, end); 
	    }

	    // etymology
	    if (etymology.Length != 0) {
		start = end;
		end.ForwardChars (etymology.Length + 1);
	    }

	    // description
	    if (description.Length != 0) {
		start = end;
		end.ForwardChars (2);
		Buffer.ApplyTag ("bold", start, end); 
		end.ForwardChars (description.Length - 1);
	    }

	    // example
	    if (example.Length != 0) {
		start = end;
		end.ForwardChars (example.Length + 1);	
		Buffer.ApplyTag ("italic", start, end); 
	    }
	}
    }
}

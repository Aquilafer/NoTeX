//
// Created by jputlock on 6/1/19.
//

#include "editor.h"

Editor::Editor() : Gtk::ApplicationWindow() {

    // Set title and border width
    this->set_title("NoTeX");
    this->set_border_width(5);

    // Allow ClickableImages to receive input
    this->set_events(Gdk::BUTTON_PRESS_MASK);
    
    //Allow for keybindings
    this->set_events( Gdk::KEY_PRESS_MASK );
    this->set_keybindings();
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &Editor::on_keypress)); 

    // Set up the textview

    // this->m_textview.set_resize_mode()
    // this->m_textview.set_size_request (400, 400);
    // this->m_textview.set_wrap_mode(Gtk::WRAP_WORD);
    // auto m_textBuffer = Gtk::TextBuffer::create();
    // m_textBuffer->set_text("Welcome to NoTeX!");
    // this->m_textview.set_buffer(m_textBuffer);

    this->init_menubar();

    // Add everything to the box
    this->m_fixed.put(this->m_menuBar, 0, 0);
    // this->m_fixed.put(this->m_textview, 0, 30);
    // this->m_fixed.put(this->m_img, 200, 200);

    // Add a container to hold everything
    this->add(this->m_fixed);

    this->show_all_children();
}

Editor::~Editor() {}

void Editor::init_menubar() {

    // Set up the menubar
    this->m_menuFile.set_label("File");
    this->m_menuBar.append(this->m_menuFile);

    // Set up file submenu
    this->m_menuFile.set_submenu(this->m_subMenuFiles);
    this->m_menuNew.set_label("New");
    this->m_menuOpen.set_label("Open");
    this->m_menuSave.set_label("Save");
    this->m_subMenuFiles.append(this->m_menuNew);
    this->m_subMenuFiles.append(this->m_menuOpen);
    this->m_subMenuFiles.append(this->m_menuSave);

    // Connect submenu to functions
    this->m_menuNew.signal_activate().connect(sigc::mem_fun(*this, &Editor::on_menu_file_new));
    this->m_menuOpen.signal_activate().connect(sigc::mem_fun(*this, &Editor::on_menu_file_open));
    this->m_menuSave.signal_activate().connect(sigc::mem_fun(*this, &Editor::on_menu_file_save));
#ifdef DEBUG
    // Debug::debug_print("Menubar finished initializing\n");
    std::cout << "Menubar finished initializing\n";
#endif
    // code here
}

void Editor::set_keybindings(){
	//initialize main variables
	char func[26][32];
	memset(func, 0, sizeof(char)*26*32);
	memset(&this->keybind_table, 0, sizeof(void (Editor::*)(void))*26);
	
	//read keybind settings from file
	char buff[64];
	std::ifstream in;
	in.open("../src/keybindings");
	if(!in){
		std::cerr << "Unable to open keybindings\n";
		return;
	}
	int i=0, j=0;
	char c;
	bool f = true;
	while(in.get(c)){
		if(isalnum(c)){
			if(c > 90){
				c -= 32;
			}
			if(f){
				buff[j] = c;
				++j;
			} else {
				strcpy(func[c-65], buff);
				f = true;
			}
		} else if(c == ':'){
			buff[j] = '\0';
			f = false;
			j = 0;
		}
	}
	in.close();
	
	//map file title to function
	for(i=0; i<26; ++i){
		//printf("%d: \"%s\"\n", i, func[i]); //check function labels
		if(strcmp(func[i], "FILENEW") == 0){
			this->keybind_table[i] = &Editor::on_menu_file_new;
		} else if(strcmp(func[i], "FILEOPEN") == 0){
			this->keybind_table[i] = &Editor::on_menu_file_open;
		} else if(strcmp(func[i], "FILESAVE") == 0){
			this->keybind_table[i] = &Editor::on_menu_file_save;
		}
	}
}


bool Editor::on_keypress(GdkEventKey *event){
	int key = -1;
	
	//ctrl + key style
	if(event->state & GDK_CONTROL_MASK){
		
		switch(event->keyval){
			case GDK_KEY_a: key=0;  break;
			case GDK_KEY_b: key=1;  break;
			case GDK_KEY_c: key=2;  break;
			case GDK_KEY_d: key=3;  break;
			case GDK_KEY_e: key=4;  break;
			case GDK_KEY_f: key=5;  break;
			case GDK_KEY_g: key=6;  break;
			case GDK_KEY_h: key=7;  break;
			case GDK_KEY_i: key=8;  break;
			case GDK_KEY_j: key=9;  break;
			case GDK_KEY_k: key=10; break;
			case GDK_KEY_l: key=11; break;
			case GDK_KEY_m: key=12; break;
			case GDK_KEY_n: key=13; break;
			case GDK_KEY_o: key=14; break;
			case GDK_KEY_p: key=15; break;
			case GDK_KEY_q: key=16; break;
			case GDK_KEY_r: key=17; break;
			case GDK_KEY_s: key=18; break;
			case GDK_KEY_t: key=19; break;
			case GDK_KEY_u: key=20; break;
			case GDK_KEY_v: key=21; break;
			case GDK_KEY_w: key=22; break;
			case GDK_KEY_x: key=23; break;
			case GDK_KEY_y: key=24; break;
			case GDK_KEY_z: key=25; break;
		}
		
		//printf("key: %d\n", key); //see when ctrl + key is pressed
		if(key != -1 && this->keybind_table[key] != NULL){
			(this->*keybind_table[key])();
		}
	}
	return true;
}

/** @brief Creates a new text buffer to work on
 *
 */
void Editor::on_menu_file_new() {
    std::cout << "Selected new file" << std::endl;

    // todo: implement actual "new file" functionality
    // ideas: hold the name of the file we're working on
    // and swap it off at this point

    NotexView* editing_window = Gtk::make_managed<NotexView>();
    this->editing_windows.push_back(editing_window);
    this->m_fixed.put(*editing_window, 0, 30);
    editing_window->set_text("Welcome to NoTeX. Please note that this program"
    " does not recognize TeX delimiters ($ $ and $$ $$) to start mathmode. "
                                        " Please use the LaTeX delimiters.");
    this->current_editing_window = editing_window;
}

/** @brief Opens a filestream to chosen file and loads it into the TextView.
 *
 */
void Editor::on_menu_file_open() {
    std::cout << "Selected open file" << std::endl;

    // todo: make tab manager to make this not go wild
    NotexView* editing_window = Gtk::make_managed<NotexView>();
    this->editing_windows.push_back(editing_window);
    this->m_fixed.put(*editing_window, 0, 30);
    this->current_editing_window = editing_window;

    Gtk::FileChooserDialog file_chooser("Select a file",
                                        Gtk::FILE_CHOOSER_ACTION_OPEN);
    file_chooser.set_transient_for(*this);

    // Add response buttons to dialog
    file_chooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    file_chooser.add_button("_Open", Gtk::RESPONSE_OK);

    // we can add filters here to only allow certain filetypes

    // actually run it and collect the result
    int result = file_chooser.run();


    switch(result) {
        case (Gtk::RESPONSE_OK): {
            std::cout << "Folder selected: " << file_chooser.get_filename()
                                             << std::endl;

            std::ifstream file_istream;
            file_istream.open(file_chooser.get_filename());

            // todo: check this section over... wrote it without access
            // to documentation
            char buffer[1000];
            std::string temp_text;
            std::string text_builder;

            while (!file_istream.eof()) {
                file_istream.read(buffer, 1000);
                temp_text = buffer;
                std::cout << temp_text << std::endl;
                text_builder += temp_text;
            }

            this->current_editing_window->set_text(text_builder);

            std::cout << "Opened from " << file_chooser.get_filename()
                                        << std::endl;

            file_istream.close();

            std::cout << "Closed filestream to " << file_chooser.get_filename()
                                                 << std::endl;

            break;
        }
        case (Gtk::RESPONSE_CANCEL): {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default: {
            std::cerr << "Unexpected action occurred in Editor::on_menu_file_open()"
                      << std::endl;
            break;
        }
    }
}

/** @brief Opens a filestream to chosen file and writes the current TextView
 *  to it.
 *
 */
void Editor::on_menu_file_save() {
    std::cout << "Selected save file" << std::endl;

    Gtk::FileChooserDialog file_chooser("Select a file",
                                        Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    file_chooser.set_transient_for(*this);

    // Add response buttons to dialog
    Gtk::Entry entry;
    // this number is arbitrary but it ok at 93?
    int entry_width = 93;
    entry.set_width_chars(entry_width);
    entry.set_max_width_chars(entry_width);
    entry.show();

    // all default response id's are in [-1, -11]
    file_chooser.add_action_widget(entry, 0);
    file_chooser.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    file_chooser.add_button("Select", Gtk::RESPONSE_OK);

    int result = file_chooser.run();

    // handle the result
    switch(result) {
        case (Gtk::RESPONSE_OK): {

            // construct the file location
            // todo: make this usable with windows filesystems
            std::string file_location = file_chooser.get_filename() + "/"
                                                            + entry.get_text();
            std::cout << "Location selected: " << file_location << std::endl;

            std::ofstream file_ostream;
            file_ostream.open(file_location);

            file_ostream << this->current_editing_window->get_text();
            std::cout << "Saved to " << file_location << std::endl;

            file_ostream.close();
            std::cout << "Closed filestream to " << file_location << std::endl;

            break;
        }
        case (Gtk::RESPONSE_CANCEL): {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default: {
            std::cerr << "Unexpected action occurred in Editor::on_menu_file_save()"
                      << std::endl;
            break;
        }
    }



}

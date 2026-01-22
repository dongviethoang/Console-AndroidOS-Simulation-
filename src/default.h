void newui() {
    if (activated) {
        cout << "Launching 2025-12-25 UI..." << el;
        sleep.delay(2000);
        #ifdef _WIN32
        int clearoldui = system("cls");
        #else
        int clearolduiunix = system("clear");
        #endif
        if (!activated) {
            cout << "Your system is not activated. Some features may be limited." << el;
            cout << "Welcome to the new UI!" << el;
        }
        else if (userin == "Developer" || userin == "Tester" && activated) {
            cout << "Welcome to the new UI, Developer/Tester!" << el;
        }
        // --- ADDED: Update the path string inside the loop ---
        while (true) {
            // filesystem::path current_path = filesystem::current_path();
            // Use generic_string() to format the path with forward slashes globally
            // string pathString = current_path.generic_string(); 
            cout << "\nD:/Console AndroidOS>";
            string newuiinput;
            getline(cin, newuiinput);
            if ((newuiinput == "apps") && (userin == "Developer" || userin == "Tester") && activated) {
                cout << "\nApps (Developer/Tester Mode):" << el;
                cout << "1. Task Manager\n";
                cout << "2. Recycle Bin\n";
                cout << "3. Firewall Settings\n";
                cout << "4. File Search\n";
                cout << "5. System Benchmark\n";
                cout << "6. Bluetooth Manager\n";
                cout << "7. Dark Mode Toggle\n";
                cout << "8. Android Defender\n";
                cout << "9. Task Scheduler\n";
                cout << "10. Hibernate Mode\n";
                cout << "11. Lock Screen\n";
                cout << "12. Exit\n";
                cout << "13. Device Manager\n";
                cout << "14. Restart\n";
                cout << "15. File Explorer\n";
                cout << "16. File Opener\n";
                cout << "17. Update\n";
                cout << "18. Activate\n";
                cout << "19. Code Editor\n";
                cout << "19.1 Launch External App\n";
                cout << "20. Text Editor\n";
                cout << "20+vim. Launch Vim Editor\n";
                cout << "21. Environment Variables\n";
                cout << "22. Play External Sound\n";
                // cout << "23. New UI mode\n";
                cout << "23. (Developer/Tester) Sign out\n";
            }
            else if (newuiinput == "apps") {
                cout << "\nApps:" << el;
                cout << "1. Task Manager\n";
                cout << "2. Recycle Bin\n";
                cout << "3. Firewall Settings\n";
                cout << "4. File Search\n";
                cout << "5. System Benchmark\n";
                cout << "6. Bluetooth Manager\n";
                cout << "7. Dark Mode Toggle\n";
                cout << "8. Android Defender\n";
                cout << "9. Task Scheduler\n";
                cout << "10. Hibernate Mode\n";
                cout << "11. Lock Screen\n";
                cout << "12. Exit\n";
                cout << "13. Device Manager\n";
                cout << "14. Restart\n";
                cout << "15. File Explorer\n";
                cout << "16. File Opener\n";
                cout << "17. Update\n";
                cout << "18. Activate\n";
                cout << "19. Code Editor\n";
                cout << "19.1 Launch External App\n";
                cout << "20. Text Editor\n";
                cout << "21. Environment Variables\n";
                cout << "22. Play External Sound\n";
            }
            else if (newuiinput == "calculator") {
                calculator();
            }
            else {
                // if constexpr (is_same_v<decltype(newuiinput), string>) {
                //    cout << el;
                // }
                // else {
                //    cerr << "Syntax error: Cannot find command '" << newuiinput << "'" << el;
                // }
                if (!newuiinput.empty() && newuiinput != "apps" && newuiinput != "calculator") {
                    cerr << "Syntax error: Cannot find command '" << newuiinput << "'" << el;
                }
            }

            if (newuiinput == "1")
            {
                task_manager();
            }
            else if (newuiinput == "2")
            {
                recycle_bin_manager();
            }
            else if (newuiinput == "3")
            {
                firewall_settings();
            }
            else if (newuiinput == "4")
            {
                file_search();
            }
            else if (newuiinput == "5")
            {
                system_benchmark();
            }
            else if (newuiinput == "6")
            {
                bluetooth_manager();
            }
            else if (newuiinput == "7")
            {
                dark_mode_toggle();
            }
            else if (newuiinput == "8")
            {
                windows_defender();
            }
            else if (newuiinput == "9")
            {
                task_scheduler();
            }
            else if (newuiinput == "10")
            {
                hibernate_mode();
            }
            else if (newuiinput == "11")
            {
                lock_screen();
            }
            else if (newuiinput == "12")
            {
                cout << "Shutting down..." << el;
                system(R"(taskkill /IM "build398.exe" /F > $null 2>&1)");
                system(R"(del /f "D:\Console AndroidOS\bin\$null")");
                clear_console();
                Beep(1000, 2000);
                cout << "It is now safe to turn off your computer." << el;
                delay(1000);
                exit(0);
            }
            else if (newuiinput == "13")
            {
                devicemgr();
            }
            else if (newuiinput == "14")
            {
                cout << "Restarting...";
                restart();
            }
            else if (newuiinput == "15")
            {
                file_explorer();
            }
            else if (newuiinput == "16")
            {
                file_opener();
            }
            else if (newuiinput == "17")
            {
                update();
            }
            else if (newuiinput == "18")
            {
                activate();
            }
            else if (newuiinput == "19")
            {
                code_editor();
            }
            else if (newuiinput == "19.1")
            {
                cout << "Are you sure you want to launch the external app? (Y/N): ";
                char conf;
                cin >> conf;
                if (conf == 'Y' || conf == 'y') {
                    security_comp = true;
                    if (security_comp) {
                        cout << "Security compromised. Android Defender has stopped the threat." << el;
                    }
                } else {
                    cout << "Cancelled launching external app." << el;
                }
            }
            else if (newuiinput == "20")
            {
                get_input2();
            }
            else if (newuiinput == "20+vim")
            {
                launch_vim();
            }
            else if (newuiinput == "21")
            {
                env_var();
            }
            else if (newuiinput == "22")
            {
                play_ext_sound();
            }
            /*
            else if (newuiinput == "exit")
            {
                break; // exit the new UI
            }
            */
            else if (newuiinput == "23" && (userin == "Developer" || userin == "Tester") && activated)
            {
                cout << "Signing out from Developer/Tester mode..." << el;
                sleep.delay(1000);
                login();
            }
            else
            {
                cout << "Invalid option." << el;
            }
        }
    }
}
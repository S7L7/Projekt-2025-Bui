//
// Created by buijo on 08.01.2026.
//
#include <iostream>
#include "adminmenu.h"
#include "DataFetch.h"

void runAdminMenu(sqlite3* db){
    int vyber = 0;

    while (true) {
        std::cout << "Vyberte možnosti 1-3 :" << std::endl
        << "1. přidat zaměstnance" << std::endl
        << "2. pozastavit zaměstnance" << std::endl
        << "3. vypsat hodiny zaměstnanců" << std:: endl
        << "4. Zpět(vypne se administrátorský mod" << std::endl;

        std::cin >> vyber;

        switch (vyber) {
            case 1:{
                std::string name,rfid;
                std::cout << "Zadejte jméno :" << std::endl;
                std::cin >> name;
                std::cout << "Zadejte RFID (max 8 znaků)" << std::endl;
                std::cin >> rfid;
                std::getline(std::cin,rfid);
                if (rfid.size() > 8) {
                     std::cout << "RFID je příliš dlouhé!" << std ::endl;
                }
                if (addEmployee(db,name,rfid)) {
                    std::cout << "Zaměstnanec úspěšně přidán!" <<std::endl;
                }else {
                    std::cout << "Zaměstnanec NEBYL přidán." << std::endl;
                }break;
            }

            case 2: {
                int id;
                std::cout << "Zadejte ID zaměstnance" << std::endl;
                std::cin >> id;
                if (deactivateEmployee(db,id)) {
                    std::cout << "Zaměstnanec byl deaktivován!" << std::endl;
                }else {
                    std::cout << "Chyba při deaktivaci zaměstnance!" << std::endl;
                }break;
            }
            case 3:     // vypsat čas
            case 4:     // zpet s vypnutim admin modu
                return;
            default : std::cout << "neplatna volba" << std::endl;
        }
    }
}

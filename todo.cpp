#include <iostream>
#include <windows.h>
#include <conio.h>

void color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

int main() 
{
    int Set[] = {7,7,7,7,7}; // Warna default
    int counter = 3;
    char key;

    for(int i=0;;) 
    {

        
        gotoxy(10,5);
        color(Set[0]);
        std::cout << "1. Lihat To-Do";

        gotoxy(10,6);
        color(Set[1]);
        std::cout << "2. Tulis To-Do";

        gotoxy(10,7);
        color(Set[2]);
        std::cout << "3. Checked To-Do";

        gotoxy(10,8);
        color(Set[3]);
        std::cout << "4. Ubah To-Do";

        gotoxy(10,9);
        color(Set[4]);
        std::cout << "5. Hapus To-Do";

        key = _getch();

        if(key == 72 && (counter >=2 && counter <= 5)) // 72 itu value dari up arrow
        {
            counter--;
        }
        if(key == 80 && (counter >=1 && counter <= 4)) 
        {
            counter++;
        }
        if(key == '\r') 
        { 
            if(counter == 1) 
            {
                // jika diklik enter dan highlight menu di nomor 1 maka program mulai dari sini
                std::cout<<"Menu 1 terpilih";
            }
            if(counter == 2) 
            {
                std::cout<<"Menu 2 Terpilih";
            }
            if(counter == 3) 
            {
                std::cout << "Menu 3 Terpilih";
            }
            if(counter == 4) 
            {
                std::cout << "Menu 4 Terpilih";
            }
            if(counter == 5) 
            {
                std::cout << "Menu 5 Terpilih";
            }
        } 

        Set[0] = 7;
        Set[1] = 7;
        Set[2] = 7;
        Set[3] = 7;
        Set[4] = 7;
        
        if(counter == 1) 
        {
            Set[0] = 12;
        }
        if(counter == 2) 
        {
            Set[1] = 12;
        }
        if(counter == 3) 
        {
            Set[2] = 12;
        }
        if(counter == 4) 
        {
            Set[3] = 12;
        }
        if(counter == 5) 
        {
            Set[4] = 12;
        }


    }
}

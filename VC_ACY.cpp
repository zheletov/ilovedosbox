﻿
#include "stdafx.h" //подключение библиотек
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>
#include <iostream>
using namespace std;
using namespace System;
using namespace System::IO;
#define ENTER 13 //обозначение кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define KEY_HOME 71
#define KEY_END 79
#define ELEMENT_COUNTER 7
struct z {
      char name[20]; //название навыка
      char range[20]; //количество целей
      char type[20]; //тип навыка
      int damage; //урон навыка
      int sp; //стоимость навыка
      char data [11]; //дата добавления навыка в игру
          }; //шаблон структуры для исходных данных 
struct sp {
          char name[20];
		  char type [20];
          int damage;
          struct sp* sled;
		  struct sp* pred;
          }; //шаблон, глобальное описание структуры двустороннего списка

int menu(int, char [7][55]); //указание шаблонов используемых функций 
void maxim(struct z*, int);
void first(struct z*, int); 
void text_data(char *,char*, int);
struct sp* vstavka (struct z* skill, struct sp* list, int it);
void kolvo (struct z*, int);
void alfalist (struct z* skill, int NC, struct sp *list);
void listing (struct z*, int);
void diagram (struct z *skill, int NC, struct sp *list);

int main(array<System::String ^> ^args)//на локальном уровне осуществляем открытие файла,
//считываем данные из файла в выделенную память 
{
char dan[7][55]={
                 "Какой навык наносит наибольшее количество урона?      ",
                 "Какой навык был добавлен в игру раньше всех?          ",
                 "Список навыков с одинаковым типом и разным уроном     ",
                 "Алфавитный список всех навыков                        ",
                 "Количество навыков, бьющих по всем врагам             ",
                 "Диаграмма. Процентное соотношение урона от навыков    ",
                 "Выход                                                 "
                }; //задание списка вопросов - пунктов меню
int NC;
char BlankLine[ ]="                                                        "; 

 int i,n;
 FILE *in,*out;
 struct z *skills;
 struct sp* list; //список
      setlocale(LC_CTYPE,"Russian"); //поддержка русского языка
      Console::CursorVisible::set(false); //отключаем видимость курсора 
      Console::BufferHeight=Console::WindowHeight; //установка размера буфера по ширине
      Console::BufferWidth=Console::WindowWidth; //установка размера буфера по высоте
      if((in=fopen("gglist.dat","r"))==NULL) //попытка открыть файл
          {
          printf("\nФайл gglist.dat не открыт !"); //при неудачном открытии выводим соообщение “Файл gglist.dat не открыт!”
          getch(); exit(1);
          }
  out=fopen("gglist.dat","w");
  fscanf(in,"%d",&NC);
  skills=(struct z*)malloc(NC*sizeof(struct z)); //выделяем память 
  for(i=0;i<NC;i++) //считываем данные из списка
	  fscanf(in,"%s%s%s%d%d%s", skills[i].name, skills[i].range, skills[i].type, &skills[i].damage, &skills[i].sp, skills[i].data);
  for(i=0;i<NC;i++) list = vstavka(skills, list, i);
  for(i=0;i<NC;i++) 
  fprintf(out,"%s %s %ld %ld %s\n",skills[i].name, 
   skills[i].range, skills[i].type, skills[i].sp,
   skills[i].data);
  for(i=0;i<NC;i++) //выводим данные из списка на экран 
	   printf("\n%-20s %-20s %-20s %4d %3d %s", skills[i].name, skills[i].range, skills[i].type, skills[i].damage, skills[i].sp, skills[i].data);
  getch();
  while(1) //в цикле создаем область для вопросов и окрашиваем еѐ в цвета
  {
	   Console::ForegroundColor=ConsoleColor::Gray;
	   Console::BackgroundColor=ConsoleColor::Blue;
	   Console::Clear();
	   Console::ForegroundColor=ConsoleColor::Blue;
	   Console::BackgroundColor=ConsoleColor::Gray;
	   Console::CursorLeft=10; //первая точка, откуда будем закрашивать область меню
	   Console::CursorTop=4;
	   printf(BlankLine);
	   for(i=0;i<7;i++) //размещение вопросов 
	   {
		   Console::CursorLeft=10;
		   Console::CursorTop=i+5;
		   printf(" %s ",dan[i]);
	   }
	   Console::CursorLeft=10; //последняя точка,где будет заканчиваться выделенная область под меню
	   Console::CursorTop=12;
	   printf(BlankLine);
	   n=menu(7,dan); //выбор вопроса в меню 
	   switch(n)
	   {
	   case 1: maxim(skills, NC); break;
	   case 2: first(skills, NC);  break;
	   case 3: listing (skills, NC);  break;
	   case 4: alfalist (skills, NC, list); break;
	   case 5: kolvo(skills, NC); break;
	   case 6: diagram (skills, NC, list);break;
	   case 7: exit(0);
       }
   } // конец while(1)...
return 0;
} // конец main()
int menu(int n,char dan[7][55]) //n=7
{
int y1=0,y2=n-1; //устанавливаем выделение для строчки 
char c=1;
while (c!=ESC) //при нажатии кнопки ESC, меню закроется 
{
switch(c) {
case DOWN: y2=y1; y1++; break; ; //программирование кнопки DOWN,которая позволяет перемещаться вниз по меню
case UP: y2=y1; y1--; break; //программирование кнопки UP,которая позволяет перемещаться вверх по меню
case ENTER: return y1+1; //программирование кнопки DOWN,которая позволяет выбрать вопрос
case KEY_HOME: { //программирование кнопки HOME,которая позволяет быстро переместиться к первому пункту меню
  if (y1 == 0) break;
  y2 = y1;
  y1 = 0;
  break;
         }
case KEY_END: { //программирование кнопки END,которая позволяет быстро переместиться к последнему пункту меню
  if (y1 == ELEMENT_COUNTER -1) break;
  y2 = y1;
  y1 = ELEMENT_COUNTER - 1;
  break;
			  }
}
if(y1>n-1){y2=n-1;y1=0;} //условие,когда срабатывает кнопка DOWN
if(y1<0) {y2=0;y1=n-1;} //условие,когда срабатывает кнопка UP
Console::ForegroundColor=ConsoleColor::White; //при выборе вопроса буквы окрашиваются в белый цвет 
Console::BackgroundColor=ConsoleColor::Gray; //привыборе вопроса,план за буквами подсвечивается серым
Console::CursorLeft=11;
Console::CursorTop=y1+5;
printf("%s",dan[y1]);
Console::ForegroundColor=ConsoleColor::Blue;
Console::BackgroundColor=ConsoleColor::Gray;
Console::CursorLeft=11;
Console::CursorTop=y2+5;
printf("%s",dan[y2]);
c=getch();
} // конец while(c!=ESC)...
exit(0);
}
// Какой навык наносит наибольший урон? (функция поиска максимума)
void maxim(struct z* skill, int NC)
{
int i=0; struct z best;
strcpy(best.name,skill[0].name); //в переменную структуры записываем нулевой элемент
best.damage=skill[0].damage;
for(i=1;i<NC;i++)
if (skill[i].damage>best.damage) //условие поиска максимального количества
{
strcpy(best.name,skill[i].name); 
best.damage=skill[i].damage;
}
Console::ForegroundColor=ConsoleColor::Gray;
Console::BackgroundColor=ConsoleColor::Blue;
Console::CursorLeft=10; //задаем область, где будет выведено макс.количество
Console::CursorTop=15;
printf("Максимальный урон %d ",best.damage); // Вывод максимального количества
Console::CursorLeft=10; //задаѐм область,где будет выведено название навыка 
Console::CursorTop=16;
printf("имеет навык %s",best.name); //Вывод названия
getch();
}
void text_data(char *s,char *sd) // функция преобразования даты
{
char s0[3],month[12][9]={
"января","февраля","марта","апреля","мая","июня",
"июля","августа","сентября","октября","ноября","декабря"};
strcpy(s,sd+8);
strcat(s," ");
strncpy(s0,sd+5,2); s0[2]=0;
strcat(s,month[ atoi(s0)-1]);
strcat(s," ");
strncat(s,sd,4);
return;
}
void first(struct z* skill, int NC) // функция поиска самой ранней даты вклада
{
int i;
char s[17];
struct z* best=skill;
for(i=1;i<NC;i++)
if (strcmp(skill[i].data,best->data)<0)
best=&skill[i];
text_data(s,best->data);
Console::ForegroundColor=ConsoleColor::Gray;
Console::BackgroundColor=ConsoleColor::Blue;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("Самый \"старый\" навык,бьющий по %s на %ld урона",
best->range,best->damage);
Console::CursorLeft=10;
Console::CursorTop=16;
printf("называется %s",best->name);
Console::CursorLeft=10;
Console::CursorTop=17;
printf("Добавлен %s ",s);
getch();
}
void kolvo(struct z *skill, int NC) // функция поиска навыков, бьющих по всем врагам 
{
int i,k=0;
for(i=0;i<NC;i++)
if (strcmp(skill[i].range,"Все")==0)
k++;
Console::ForegroundColor=ConsoleColor::Gray;
Console::BackgroundColor=ConsoleColor::Blue;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("Навыков, бьющих по всем врагам");
printf("\nВсего : %d",k);
getch();
}
struct sp* vstavka(struct z* skill, struct sp* list, int it) {  //Вставка в список
    struct sp *nov, *nt, *z = 0;
    for (nt = list; nt != 0 && strcmp(nt->type, skill[it].type) < 0; z = nt, nt = nt->sled);
    nov = (struct sp*)malloc(sizeof(struct sp));
    strcpy(nov->name, skill[it].name);
    strcpy(nov->type, skill[it].type);
	nov->damage = skill[it].damage;
    nov->pred = z;
	nov->sled = nt;

    if (!z)
        list = nov;
    else
        z->sled = nov;

	if (nt)
  nt->pred = nov;
    return list;
}

void alfalist(struct z* skill, int NC, struct sp *list)//Формирование списка
{
int i;
struct sp* nt,*z = 0;
Console::ForegroundColor=ConsoleColor::Gray;
Console::BackgroundColor=ConsoleColor::Blue;
Console::Clear();
printf("\n Алфавитный список навыков");
printf("\n ===============================\n");
for (nt = list; nt != 0; nt = nt->sled)
printf("\n %-20s %-20s %ld",nt->name, nt->type, nt->damage);
Console::CursorTop = 4;
 for (nt = list; nt != 0; z = nt, nt = nt->sled);
 for (nt = z; nt != 0; nt = nt->pred) {
  Console::CursorLeft = 42;
  printf("%-18s  %-14s \n", nt->name, nt->type, nt->damage);
 }
getch();
}
void listing(struct z* skill, int NC) { // Навыки с одним уроном, но разным типом (Сложный вопрос - поиск соответствия)
    int i, j, k;
    for (i = 0; i < NC; i++) {
        if (!k) {
            for (j = i + 1; j < NC; j++) {
                if ((skill[i].damage == skill[j].damage) && (skill[i].type != skill[j].type)) {
                    Console::ForegroundColor=ConsoleColor::Gray;
                    Console::BackgroundColor=ConsoleColor::Blue;
                    Console::CursorLeft = 10;
                    Console::CursorTop = 15;
                    printf("Навыки %s и %s", skill[i].name, skill[j].name);
                    Console::CursorLeft = 10;
                    Console::CursorTop = 16;
                    printf("оба наносят %ld урона", skill[i].damage);
                    Console::CursorLeft = 10;
                    Console::CursorTop = 17;
                    printf("но имеют разные типы(%s и %s)", skill[i].type, skill[j].type);
                    k = 1;
                    break;
                }
            }
        }
    }
    if (!k) {
           Console::ForegroundColor=ConsoleColor::Gray;
           Console::BackgroundColor=ConsoleColor::Blue;
        Console::CursorLeft = 10;
        Console::CursorTop = 15;
        printf("Совпадений не найдено.");
    }
    getch();
}
void diagram(struct z *skill, int NC, struct sp *list) // Диаграмма, показывающая соотношение урона между навыками 
{
struct sp *nt;
int len,i,NColor;
long sum = 0 ;
char str1[20];
char str2[20];
System::ConsoleColor Color;
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor=ConsoleColor::White;
Console::Clear();
for(i=0;i<NC;i++) sum = sum+skill[i].damage ;
if(!list)
for(i=0;i<NC;i++)
Color=ConsoleColor::Black; NColor=0;
for(nt=list,i=0; nt!=0; nt=nt->sled,i++)
{
sprintf(str1,"%s",nt->name);
sprintf(str2,"%3.1f%%",(nt->damage*100./sum));
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor= ConsoleColor::White;
Console::CursorLeft=5; Console::CursorTop=i+1;
printf(str1);
Console::CursorLeft=20;
printf("%s",str2);
Console::BackgroundColor=++Color; NColor++;
Console::CursorLeft=30;
for(len=0; len<nt->damage*100/sum; len++) printf(" ");
if(NColor==14)
{ Color=ConsoleColor::Black; NColor=0; }
}
getch();
return ;
}

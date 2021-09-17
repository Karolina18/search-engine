#include "crawler.h"
#include "db.h"
#include <iostream>
#include <string.h>
#ifdef _WINDOWS
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

using namespace WSE;
using namespace std;

void Crawler::traversal()
{
    srand(time(NULL));

    string titles = "";
    string descriptions = "";
    string url;
    int x = 1;

   while(1)
    {
        // generate next url
        url = "https://opticbox.ru/solntsezashchitnye-ochki";
        url = url + "?page=" + to_string(x);
        x++;


        string html = request(url.c_str());//отправка запроса (стиль строки си)




        if( is_content(html) )//проверка на наличие информации
        {
            titles = getTitles(html);
            descriptions = getDescriptions(html);
        }


        else
        {
            break;
        }

        updateDb(titles, descriptions);


    }

}

bool Crawler::updateDb(string titles, string descriptions)
{
    DB db;

    string title = "";
    string descr = "";

    int titIndex = 0;
    int desIndex = 0;

    for (int i = 0; i < 36; i ++)
    {
        while(titles.compare(titIndex, 1, "\n", 0, 1) != 0)
        {
            title += titles[titIndex ++];
        }

        while(descriptions.compare(desIndex, 1, "\n", 0, 1) != 0)
        {
            descr += descriptions[desIndex ++];
        }

        db.insert(title, descr);

        titIndex ++;
        desIndex ++;
        title = "";
        descr = "";
    }

    return true;
}


string Crawler::getTitles(string html)
{
    string title = "";
    int count = 0;

    //const string className = "catProductTitle";
    const string className = "list_item__caption";
    const string less = "<";
    const string more = ">";
    const int length = html.length();


    for(int i = 0; i < length; i ++)
    {
        if(html.compare(i, 18, className, 0, 18) == 0)
        {
            count++;
        }

        if(count > 0 && html.compare(i, 1, more, 0, 1) == 0)
        {
            count++;
        }

        if(count == 3 && html.compare(i, 1, more, 0, 1) != 0)
        {

            if (html.compare(i, 1, less, 0, 1) == 0)
            {
                count = 0;
                title += "\n";

                continue;
            }

            title += html[i];
        }
    }

    return title;
}


string Crawler::getDescriptions(string html)
{
    const int length = html.length();
  //  const string className = "product_s_desc";
    const string className = "list_item__bottom__price";
    const string more  = ">";
    const string less  = "<";
    string desc  = "";
    int count = 0;

    for (int i = 0; i < length; i ++)
    {
        if (html.compare(i, 24, className, 0, 24) == 0)
        {
            count ++;
        }

        if (count > 0 && html.compare(i, 1, more, 0, 1) == 0)
        {
            count ++;
        }

        if (count == 2 && html.compare(i, 1, more, 0, 1) != 0)
        {
            if (html.compare(i, 1, less, 0, 1) == 0)
            {
                count = 0;
                desc = n_trim(desc);
                desc += "\n";

                continue;
            }

            desc += html[i];
        }
    }

    return desc;
}




bool Crawler::is_content(string html) //есть ли еще информация
{
    string titles = getTitles(html);

    if (titles.length() > 0)
        return true;
    else
        return false;
}


string Crawler::n_trim(const string& str)
{
    size_t first = str.find_first_not_of("\n");
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of("\n");
    return str.substr(first, (last - first + 1));
}

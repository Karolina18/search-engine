#include "httpClient.h"
#include <string>

using namespace std;

namespace WSE
{
    class Crawler: public HttpClient
            {
            private:
                string getTitles(string html);
                string getDescriptions(string html);
                string n_trim(const string& str);
                int random(int min, int max);
                bool is_content(string html);
                bool updateDb(string titles, string descriptions);

            public:
                void traversal();

            };
}
#include <string>

using namespace std;

namespace WSE
{
    class DB
            {
            public:
                bool create();
                bool insert(string title, string description);
                bool search(string query);
            };
}


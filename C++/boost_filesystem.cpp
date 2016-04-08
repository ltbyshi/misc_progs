#include <iostream>
using namespace std;
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#define PRINTEXPR(expr) cout << #expr << " => " << expr << endl;
int main()
{
    path pth1("/root/directory/file.ext");
    PRINTEXPR(pth1);
    PRINTEXPR(pth1.root_name());
    PRINTEXPR(pth1.root_directory());
    PRINTEXPR(pth1.root_path());
    PRINTEXPR(pth1.relative_path());
    PRINTEXPR(pth1.parent_path());
    PRINTEXPR(pth1.filename());
    PRINTEXPR(pth1.extension());
    PRINTEXPR(pth1.stem());
    PRINTEXPR(pth1.replace_extension(".pdf"));
    PRINTEXPR(pth1.remove_filename());
    PRINTEXPR((pth1 /= "newfile"));
    PRINTEXPR((pth1 += ".doc"));
    PRINTEXPR(exists("/tmp/junk"));
    PRINTEXPR(create_directory("/tmp/junk"));

    return 0;
}

#include "Dir.h"

Dir::Dir(std::string path) :
	File(path) {

}
#ifdef WINDOWS
bool Dir::mkdir() {
	return CreateDirectory((path.c_str()), 0);
}
#else
bool Dir::mkdir() {
        return ::mkdir(path.c_str(), 0700);
}
#endif
#ifdef WINDOWS
std::vector<File> Dir::list() const {
	std::vector<File> s;
	if (isDir()) {
		WIN32_FIND_DATA d;
		HANDLE h = FindFirstFile(((path + "/*").c_str()), &d);
		if (h != INVALID_HANDLE_VALUE) {
			do {
				if (strcmp((char*)d.cFileName, ".") && strcmp((char*)d.cFileName, ".."))
					s.push_back(File(path + "/" + (char*)d.cFileName));
			} while (FindNextFile(h, &d));
			FindClose(h);
		}
	}
	return s;
}

size_t Dir::countRecursive()
{
	size_t count = 0;
	for (File& f : list())
	{
		if (f.isDir())
		{
			count += Dir(f.path).countRecursive();
		} else
		{
			count++;
		}
	}
	return count;
}
#else
std::vector<File> Dir::list() const {
        std::vector<File> s;
        if (isDir()) {
                DIR* d = opendir(path.c_str());
                dirent* p = 0;
                while (p=readdir(d)) {
                	s.push_back(File((char*)p->d_name));
                }
		closedir(d);
        }
        return s;
}
#endif

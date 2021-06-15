#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>


using namespace std;

vector<string> get_filenames(const string & directory_name)
{
    filesystem::path path(directory_name);
    vector<string> filenames;

    const filesystem::directory_iterator end{};

    for (filesystem::directory_iterator itr{path}; itr != end; ++itr)
        filenames.push_back(itr->path().string());

    return filenames;
}

int get_lines(const string & filename)
{
    int lines = 0;
    string buffer;

    ifstream in(filename);
    if (!in.is_open()) {
        cout << "The file could not be opened\n";
        exit(1);
    }

    while (getline(in, buffer))
        ++lines;

    return lines;
}

string get_extension(const string & filename)
{
    const size_t period_loc = filename.find('.');

    if (period_loc == string::npos)
            return "directory";
    else
        return filename.substr(period_loc, filename.length());
}

int search_directory(const string & path, map<string, int> & extensions) {
    vector<string> filenames = get_filenames(path);
    string extension;
    int files_searched = 0;

    for (const string & filename: filenames) {
        if (filename.find("node_modules") != string::npos)
            continue;

        extension = get_extension(filename);

        if (extension == "directory") {
            files_searched += search_directory(filename, extensions);
            continue;
        } else if (extension == ".DS_Store" || extension == ".json" || extension == ".gitignore" || extension == ".example.json")
            continue;
        else
            extensions[extension] += get_lines(filename);

        ++files_searched;
    }
    return files_searched;
}

int main() {
    string client = "/Users/flouid/Downloads/project-monitor-louis-dev/client/src";
    map<string, int> extensions;
    int files_searched = search_directory(client, extensions);

    cout << "Client:" << endl;
    for (const auto & extension: extensions)
        cout << "Found " << extension.second << " lines of " << extension.first << endl;
    cout << "Searched " << files_searched << " files" << endl;

    string server = "/Users/flouid/Downloads/project-monitor-louis-dev/server";
    extensions.clear();
    files_searched = search_directory(server, extensions);
    cout << endl << "Server:" << endl;
    for (const auto & extension: extensions)
        cout << "Found " << extension.second << " lines of " << extension.first << endl;
    cout << "Searched " << files_searched << " files" << endl;

    return 0;
}
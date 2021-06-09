#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>


using namespace std;

vector<string> get_filenames(const string & directory_name)
{
    filesystem::path path(directory_name);
    vector<string> filenames;

    const filesystem::directory_iterator end{};

    for (filesystem::directory_iterator itr{path}; itr != end; ++itr) {
        filenames.push_back(itr->path().string());
    }

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

    while (getline(in, buffer)) {
        ++lines;
    }

    return lines;
}

string get_extension(const string & filename) {
    const size_t period_loc = filename.find('.');

    if (period_loc == string::npos) {
            return "directory";
    }
    else {
        return filename.substr(period_loc, filename.length());
    }
}

void search_directory(const string & path, int & js_lines, int & scss_lines) {
    vector<string> filenames = get_filenames(path);
    string extension;

    for (const string & filename: filenames) {
        if (filename == "node_modules") {
            continue;
        }

        extension = get_extension(filename);

        if (extension == "directory") {
            search_directory(filename, js_lines, scss_lines);
        }
        else if (extension == ".js") {
            js_lines += get_lines(filename);
        }
        else if (extension == ".scss") {
            scss_lines += get_lines(filename);
        }

    }
}

int main() {
    string client = "/Users/flouid/Documents/atl-project-monitor/client/src";
    int js_lines = 0;
    int scss_lines = 0;
    search_directory(client, js_lines, scss_lines);

    cout << "Client:" << endl;
    cout << "Found " << js_lines << " lines of javascript." << endl;
    cout << "Found " << scss_lines << " lines of scss." << endl;

    return 0;
}
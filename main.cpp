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

int search_directory(const string & path, map<string, int[2]> & extensions) {
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
        else {
            extensions[extension][0] += get_lines(filename);
            ++extensions[extension][1];
        }

        ++files_searched;
    }
    return files_searched;
}

int main() {
    int sum_lines = 0;

    string client = "/Users/flouid/Downloads/project-monitor-louis-dev/client/src";
    map<string, int[2]> client_extensions;
    int client_files_searched = search_directory(client, client_extensions);

    cout << "Client:" << endl;
    for (const auto & extension: client_extensions) {
        cout << "\tFound " << extension.second[0] << " lines of " << extension.first << " across " << extension.second[1] << " files" << endl;
        sum_lines += extension.second[0];
    }
    cout << "\tSearched " << client_files_searched << " files" << endl;

    string server = "/Users/flouid/Downloads/project-monitor-louis-dev/server";
    map<string, int[2]> server_extensions;
    int server_files_searched = search_directory(server, server_extensions);
    cout << endl << "Server:" << endl;
    for (const auto & extension: server_extensions) {
        cout << "\tFound " << extension.second[0] << " lines of " << extension.first << " across " << extension.second[1] << " files" << endl;
        sum_lines += extension.second[0];
    }
    cout << "\tSearched " << server_files_searched << " files" << endl;

    cout << endl << "Total:" << endl;
    cout << "\tFound " << sum_lines << " lines across " << client_files_searched + server_files_searched << " files" << endl;

    return 0;
}
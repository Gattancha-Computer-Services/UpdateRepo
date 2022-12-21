#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "json/json.h"

using namespace std;

//Declare Variables
string distro;
string rockylinux = "Rocky";
string centoslinux = "CentOS";
string repo;
string RepoUrl;
string RepoPrefix;
string ConfigFile = "/etc/updaterepo/repoconf.json";

//Declare Functions
string cfgURL();
string cfgPrefix();
void copyRepo(string repo);
string toUpper(string text);

//Main Function
int main()
{
        if (getuid())
        {
                printf("\n");
                printf("ERROR\n");
                printf("-----\n");
                printf("\n");
                printf("%s", "You are not authorised to perform this action!\n");
                printf("Please contact your System Administrator\n");
                exit(EXIT_FAILURE);
        }

        ifstream releaseFile("/etc/system-release");

        getline(releaseFile, distro);

        if (distro.find(rockylinux)!=string::npos)
        {
                repo = "rocky";
                copyRepo(repo);
                return 0;
        }
        else if (distro.find(centoslinux)!=string::npos)
        {
                repo = "centos";
                copyRepo(repo);
                return 0;
        }
        else
        {
                cout << "Not a RHEL based Distro: " + distro + "\n";
                return 1;
        }

        releaseFile.close();
}

//Copy the repo from remote server
void copyRepo(string repo)
{
        string RepoUrl = cfgURL();
        string RepoPrefix = cfgPrefix();
        string repoUpper = toUpper(repo);
        string repofile = RepoPrefix + "-" + repoUpper +".repo";

        cout << "Installing latest " + RepoPrefix + " Repo for " + repoUpper + " Linux" << endl;
        string cmdDNF("/usr/bin/dnf config-manager --add-repo=" + RepoUrl + "/" + repofile);
        string cmdRename("/usr/bin/mv /etc/yum.repos.d/" + repofile + "  /etc/yum.repos.d/" + RepoPrefix + ".repo");
        system(cmdDNF.c_str());
        system(cmdRename.c_str());
}

//Convert to Uppercase
string toUpper(string text)
{
        for (int x = 0; x < text.length(); x++)
        {
                if (x == 0)
                {
                        text[x] = toupper(text[x]);
                }
                else if (text[x -1] == ' ')
                {
                        text[x] = toupper(text[x]);
                }
        }

        return text;
}

//Get Repo URL from config file
string cfgURL()
{
        ifstream file(ConfigFile);
        Json::Value configJson;
        Json::Reader reader;

        reader.parse(file, configJson);

        RepoUrl = configJson["url"].asString();

        return 0;
}

//Get Repo Prefix from config file
string cfgPrefix()
{
        ifstream file(ConfigFile);
        Json::Value configJson;
        Json::Reader reader;

        reader.parse(file, configJson);

        RepoPrefix= configJson["repo-prefix"].asString();

        return 0;
}
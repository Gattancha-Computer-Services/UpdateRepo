#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

string distro;
string rockylinux = "Rocky";
string centoslinux = "CentOS";
string repo;

void copyRepo(string repo);
string toUpper(string text);
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

void copyRepo(string repo)
{
        string repoUpper = toUpper(repo);
        string repofile = "GCS-" + repoUpper +".repo";
        cout << "Installing latest GCS Repo for " + repoUpper + " Linux" << endl;
        string cmdDNF("/usr/bin/dnf config-manager --add-repo=http://yum.gattancha.co.uk/"+repofile);
        string cmdRename("/usr/bin/mv /etc/yum.repos.d/"+repofile+"  /etc/yum.repos.d/GCS.repo");
        system(cmdDNF.c_str());
        system(cmdRename.c_str());

}

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

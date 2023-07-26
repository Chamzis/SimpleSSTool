#include "../../Logger/Logger.h"
#include "../../SimpleSSTool.h"
#include "../../Utility/Utility.h"
#include "../Check.h"
#include "../../miniz/miniz.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <filesystem>
#include <future>
#include <TlHelp32.h>
#include <sddl.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

void checks::start_xray_check()
{
    logger("Starting XRay Check...", log_type::INFO);

    // Basic check
    bool p1 = true;
    vector<string> listFiles;
    char* appdata_path;
    size_t len;
    errno_t err = _dupenv_s(&appdata_path, &len, "APPDATA");
    std::string s = appdata_path;
    std::string res_path = s + "\\.minecraft\\resourcepacks";
    listFiles = util::files_in_folder(res_path.c_str());

    if (listFiles.size() != 0)
        for (string s : listFiles)
            if (s.find("xray") != std::string::npos || s.find("Xray") != std::string::npos || s.find("XRay") != std::string::npos) {
                logger("Found XRay in %appdata% (.zip)", log_type::WARNING);
                p1 = false;
            }

    std::vector<std::filesystem::path> folderList;
    for (const auto& entry : std::filesystem::directory_iterator(res_path.c_str())) {
        if (entry.is_directory()) {
            folderList.push_back(entry.path());
        }
    }

    if (folderList.size() != 0)
        for (std::filesystem::path pa : folderList)
            if (pa.string().find("xray") != std::string::npos || pa.string().find("Xray") != std::string::npos || pa.string().find("XRay") != std::string::npos) {
                logger("Found XRay in %appdata% (folder)", log_type::WARNING);
                p1 = false;
            }


    if (p1)
        checks::passed++;

    // Advance check
    /*
    This checks contents of every .zip ending in .json and checks the size.
    If the size is low then it flags.
    It will flag some legit resourcepacks but it will definitly find XRay.
    */

    bool p2 = true;

    if (listFiles.size() != 0) {
        for (string s : listFiles) {
            string path = res_path + "\\" + s; // Path of the resource pack
            
            // Using miniz to retreive the files
            mz_zip_archive zipArchive;
            mz_bool status;
            mz_zip_archive_file_stat fileStat;

            memset(&zipArchive, 0, sizeof(zipArchive)); // This memset cost me 2 hours of my sanity :c

            status = mz_zip_reader_init_file(&zipArchive, path.c_str(), 0);
            if (!status) {
                cout << "Failed to load " << path << " (file not zip?)" << endl;
                return;
            }

            int numFiles = mz_zip_reader_get_num_files(&zipArchive);
            for (int i = 0; i < numFiles; ++i) {
                status = mz_zip_reader_file_stat(&zipArchive, i, &fileStat);
                if (!status) {
                    logger("Failed to get info from the file...", log_type::ERR);
                    mz_zip_reader_end(&zipArchive);
                    return;
                }

                std::string fileName = fileStat.m_filename;
                if (fileStat.m_uncomp_size < 1000000 && (fileName.size() > 5 && fileName.substr(fileName.size() - 5) == ".json")) {
                    logger("Found XRay in %appdata% (file size)", log_type::WARNING);
                    cout << "Name of resourcepack to check: " << s << endl;
                    break;
                }
            }

            mz_zip_reader_end(&zipArchive);
        }
    }

    if (p2)
        checks::passed++;
}
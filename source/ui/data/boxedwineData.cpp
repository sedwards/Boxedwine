#include "boxedwine.h"
#include "../boxedwineui.h"

std::vector<BoxedContainer*> BoxedwineData::containers;
std::vector<BoxedWinVersion> BoxedwineData::winVersions;

BoxedWinVersion::BoxedWinVersion(const std::string& ver, const std::string& desc, U32 major, U32 minor, U32 build, U32 id, const std::string& csd, U16 majorPack, U16 minorPack, const std::string& product) : szVersion(ver), szDescription(desc), dwMajorVersion(major), dwMinorVersion(minor), dwBuildNumber(build), dwPlatformId(id), szCSDVersion(csd), wServicePackMajor(majorPack), wServicePackMinor(minorPack), szProductType(product) {
}

void BoxedwineData::init(int argc, const char **argv) {
    GlobalSettings::init(argc, argv);

    BoxedwineData::winVersions.push_back(BoxedWinVersion("win10", "Windows 10", 10, 0, 0x42EE, VER_PLATFORM_WIN32_NT, "", 0, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win81", "Windows 8.1", 6, 3, 0x2580, VER_PLATFORM_WIN32_NT, "", 0, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win8", "Windows 8", 6, 2, 0x23F0, VER_PLATFORM_WIN32_NT, "", 0, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win2008r2", "Windows 2008 R2", 6, 1, 0x1DB1, VER_PLATFORM_WIN32_NT, "Service Pack 1", 1, 0, "ServerNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win7", "Windows 7", 6, 1, 0x1DB1, VER_PLATFORM_WIN32_NT, "Service Pack 1", 1, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win2008", "Windows 2008", 6, 0, 0x1772, VER_PLATFORM_WIN32_NT, "Service Pack 2", 2, 0, "ServerNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("vista", "Windows Vista", 6, 0, 0x1772, VER_PLATFORM_WIN32_NT, "Service Pack 2", 2, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win2003", "Windows 2003", 5, 2, 0xECE, VER_PLATFORM_WIN32_NT, "Service Pack 2", 2, 0, "ServerNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("winxp", "Windows XP", 5, 1, 0xA28, VER_PLATFORM_WIN32_NT, "Service Pack 3", 3, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win2k", "Windows 2000", 5, 0, 0x893, VER_PLATFORM_WIN32_NT, "Service Pack 4", 4, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("winme", "Windows ME", 4, 90, 0xBB8, VER_PLATFORM_WIN32_WINDOWS, " ", 0, 0, ""));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win98", "Windows 98", 4, 10, 0x8AE, VER_PLATFORM_WIN32_WINDOWS, " A ", 0, 0, ""));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win95", "Windows 95", 4, 0, 0x3B6, VER_PLATFORM_WIN32_WINDOWS, "", 0, 0, ""));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("nt40", "Windows NT 4.0", 4, 0, 0x565, VER_PLATFORM_WIN32_NT, "Service Pack 6a", 6, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("nt351", "Windows NT 3.51", 3, 51, 0x421, VER_PLATFORM_WIN32_NT, "Service Pack 5", 5, 0, "WinNT"));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win31", "Windows 3.1", 3, 10, 0, VER_PLATFORM_WIN32s, "Win32s 1.3", 0, 0, ""));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win30", "Windows 3.0", 3, 0, 0, VER_PLATFORM_WIN32s, "Win32s 1.3", 0, 0, ""));
    BoxedwineData::winVersions.push_back(BoxedWinVersion("win20", "Windows 2.0", 2, 0, 0, VER_PLATFORM_WIN32s, "Win32s 1.3", 0, 0, ""));    
}

void BoxedwineData::startApp() {
    GlobalSettings::startUpArgs.apply();
}

void BoxedwineData::reloadContainers() {
    loadContainers();
    GlobalSettings::reloadApps();
}

void BoxedwineData::loadContainers() {
     for (auto &container : BoxedwineData::containers) {
        delete container;
    }
    BoxedwineData::containers.clear();
    Fs::iterateAllNativeFiles(GlobalSettings::getContainerFolder(), false, true, [] (const std::string& filepath, bool isDir)->U32 {
        if (isDir) {
            BoxedContainer* container = new BoxedContainer();
            if (container->load(filepath)) {
                BoxedwineData::containers.push_back(container);
            } else {
                delete container;
            }
        }
        return 0;
    });
    BoxedwineData::sortContainers();
}

void BoxedwineData::loadUI() {
    loadContainers();
}

BoxedContainer* BoxedwineData::getContainerByDir(const std::string& dir) {
    for (auto& container : BoxedwineData::containers) {
        if (container->getDir()==dir) {
            return container;
        }
    }
    return NULL;
}

void BoxedwineData::addContainer(BoxedContainer* container) { 
    BoxedwineData::containers.push_back(container); 
    BoxedwineData::sortContainers();
}

void BoxedwineData::sortContainers() {
    std::sort(BoxedwineData::containers.begin(), BoxedwineData::containers.end(), [](BoxedContainer* a, BoxedContainer* b) { 
        return a->getName() < b->getName(); 
        });
}
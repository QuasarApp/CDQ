#include "qif.h"
#include "quasarapp.h"
#include "deploycore.h"
#include "deployconfig.h"

#include <QDateTime>
#include <QProcess>
#include <packagecontrol.h>
#include <pathutils.h>

QIF::QIF(FileManager *fileManager)
    :iDistribution(fileManager){

    setLocation("tmp QIF");
};

Envirement QIF::toolKitEnv() const {
    Envirement result;

    if (QuasarAppUtils::Params::isEndable("qifFromSystem")) {
        result.addEnv(QProcessEnvironment::systemEnvironment().value("PATH"));

        // BASE
        const DeployConfig *cfg = DeployCore::_config;
        auto basePATH = cfg->qtDir.getBins() + "/../../../Tools/QtInstallerFramework/";
        QDir QifDir(basePATH);
        auto list = QifDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        QMap<double, QString> sortedItems;
        for (const auto& i : list) {
            sortedItems.insert(i.toDouble(), i);
        }

        if (sortedItems.size()) {
            basePATH += ("/" + sortedItems.last() + "/bin");
            result.addEnv(basePATH);
        }

        return result;
    }



    // SNAP

    QString AppPath = QuasarAppUtils::Params::getStrArg("appPath", "");
    result.addEnv(AppPath + "/../QIF/");

    //Installer
    result.addEnvRec(AppPath + "/../../QIF/", 2);

    return result;
}

QString QIF::runCmd() {
    QString base = "binarycreator";

    if (binarycreator.isEmpty())
        binarycreator = DeployCore::findProcess(toolKitEnv().concatEnv(), base);

    if (binarycreator.isEmpty())
        return base;

    return binarycreator;
}

bool QIF::deployTemplate(PackageControl &pkg) {
    auto customTemplate = QuasarAppUtils::Params::getStrArg("qif", "");
    const DeployConfig *cfg = DeployCore::_config;

    QStringList sufixes = {
        "js", "qs", "xml"
    };

    if (customTemplate.isEmpty()) {
        // default template

        for (auto it = cfg->packages().begin();
             it != cfg->packages().end(); ++it) {
            auto package = it.value();


            TemplateInfo info;
            info.Name = PathUtils::stripPath(it.key());
            bool fDefaultPakcage = cfg->getDefaultPackage() == info.Name;

            if (fDefaultPakcage) {
                QFileInfo targetInfo(*package.targets().begin());
                info.Name = targetInfo.baseName();
            }

            if (!package.name().isEmpty()) {
                info.Name = package.name();
            }

            auto location = cfg->getTargetDir() + "/" + getLocation() + "/packages/" + info.Name;
            auto locationData = location + "/data";
            if (cfg->getDefaultPackage() != info.Name) {
                locationData += "/" + info.Name;
            }

            info.Description = "This package contains the " + info.Name;
            if (!package.description().isEmpty())
                info.Description = package.description();

            info.Version = "1.0";
            if (!package.version().isEmpty())
                info.Version = package.version();

            info.ReleaseData = QDate::currentDate().toString("yyyy-MM-dd");
            if (!package.releaseData().isEmpty())
                info.ReleaseData = package.releaseData();

            info.Icon = "icons/Icon.png";
            if (package.icon().isEmpty()) {
                if (!copyFile(":/Templates/QIF/Distributions/Templates/qif/Icon.png",
                              locationData + "/icons/", false)) {
                    return false;
                }
            } else {
                QFileInfo iconInfo(package.icon());
                info.Icon = info.Name + "/icons/" + iconInfo.fileName();
                if (!copyFile(package.icon(), locationData + "/icons/", false)) {
                    return false;
                }
            }

            info.Publisher = "Company";
            if (!package.publisher().isEmpty())
                info.Publisher = package.publisher();

            QString cmdArray = "[";
            int initSize = cmdArray.size();
            for (const auto &target :package.targets()) {
                auto fileinfo =  QFileInfo(target);
                if (fileinfo.suffix().compare("exe", ONLY_WIN_CASE_INSENSIATIVE) == 0 || fileinfo.suffix().isEmpty()) {
                    if (cmdArray.size() > initSize) {
                        cmdArray += ",";
                    }
                    cmdArray += "\"" + info.Name + "/" + fileinfo.fileName() + "\"";
                }
            }
            cmdArray += "]";

            info.Custom = {{"[\"array\", \"of\", \"cmds\"]", cmdArray},
                           {"$LOCAL_ICON", info.Name + "/icons/" + QFileInfo(info.Icon).fileName()}};


            if (info.Name.isEmpty()) {
                info.Name = "Application";
            }

            if (!unpackDir(":/Templates/QIF/Distributions/Templates/qif/packages/default",
                           location, info, sufixes)) {
                return false;
            }

            if (!pkg.movePackage(it.key(), locationData)) {
                return false;
            }

            if (fDefaultPakcage)
                generalInfo = info;

        }

        auto configLocation = cfg->getTargetDir() + "/" + getLocation() + "/config/";

        auto qifStyle = getStyle(QuasarAppUtils::Params::getStrArg("qifStyle", ""));
        auto qifBanner = QuasarAppUtils::Params::getStrArg("qifBanner", "");
        auto qifLogo = QuasarAppUtils::Params::getStrArg("qifLogo", "");

        auto configTemplate = ":/Templates/QIF/Distributions/Templates/qif/config/";
        if (qifStyle.size() || qifBanner.size() || qifLogo.size()) {
            configTemplate = ":/Templates/QIF/Distributions/Templates/qif/config custom designe/";
        }

        if (!unpackDir(configTemplate,
                       configLocation, generalInfo, sufixes)) {
            return false;
        }

        if (qifStyle.size() && !copyFile(qifStyle, configLocation + "/style.css", true)) {
            return false;
        }

        if (qifBanner.size() && !copyFile(qifBanner, configLocation + "/banner.png", true)) {
            return false;
        }

        if (qifLogo.size() && !copyFile(qifLogo, configLocation + "/logo.png", true)) {
            return false;
        }

        return true;
    }

    // custom template

    return true;
}

QStringList QIF::runArg() const {

    auto location = DeployCore::_config->getTargetDir() + "/" + getLocation();

    return {
        "-c", location + "/config/config.xml",
                "-p", location + "/packages/",
                "-v",
                installerFile()
    };
}

bool QIF::removeTemplate() const {
    auto customTemplate = QuasarAppUtils::Params::getStrArg("qif", "");
    const DeployConfig *cfg = DeployCore::_config;

    registerOutFiles();
    if (customTemplate.isEmpty()) {
        return QDir(cfg->getTargetDir() + "/" + getLocation()).removeRecursively();
    }

    return true;
}

QProcessEnvironment QIF::processEnvirement() const {
    return QProcessEnvironment::systemEnvironment();
}

QStringList QIF::outPutFiles() const {
    return {installerFile()};
}

QString QIF::getStyle(const QString& input) const {
    QDir resurces(":/Styles/Distributions/Templates/qif/Styles");
    auto list = resurces.entryInfoList(QDir::Files);
    for (const auto& style : list) {
        if (input == style.baseName()) {
            return style.absoluteFilePath();
        }
    }

    QFileInfo f(input);

    if (f.isFile()) {
        return f.absoluteFilePath();
    }

    QuasarAppUtils::Params::log(input +  " not exits",
                                QuasarAppUtils::Error);

    return "";
}

QString QIF::installerFile() const {
#ifdef Q_OS_LINUX
    QString sufix = ".run";
#else
    QString sufix = ".exe";
#endif

    return DeployCore::_config->getTargetDir() + "/Installer" + generalInfo.Name + sufix;
}


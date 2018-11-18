/*
 * Copyright (C) 2018 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#ifndef DEPLOY_H
#define DEPLOY_H
#include <QDir>
#include <QString>
#include <QStringList>
#include "utils.h"

class Deploy {
  private:
    bool deployQml = false;
    bool onlyCLibs = false;
    int depchLimit = 0;
    QString qmlScaner = "";
    QString qmake = "";
    QString qtDir = "";
    QString target = "";
    QString targetDir = "";
    QString qmlDir = "";

    QStringList QtLibs;
    QStringList noQTLibs;
    QStringList qmlLibs;
    QStringList neededPlugins;
    QStringList ignoreList;
    QStringList extraPath;
    QStringList extraPlugins;

    bool isQtLib(const QString &lib) const;

    void copyFiles(const QStringList &files, const QString &target);
    bool copyFile(const QString &file, const QString &target,
                  QStringList *mask = nullptr);
    void extract(const QString &file, bool isExtractPlugins = true);
    QString recursiveInvairement(int depch, QDir &dir);
    void extractPlugins(const QString &lib);
    bool copyPlugin(const QString &plugin);
    void copyPlugins(const QStringList &list);
    bool copyFolder(QDir &from, QDir &to, const QString &filter = "",
                    QStringList *listOfCopiedItems = nullptr,
                    QStringList *mask = nullptr);

    bool extractQml();

    void strip(const QString &dir);

    QStringList extractImportsFromDir(const QString &dirpath);
    QStringList findFilesInsideDir(const QString &name, const QString &dirpath);
    QStringList extractImportsFromFiles(const QStringList &filepath);
    bool extractQmlAll();
    bool extractQmlFromSource(const QString sourceDir);
    QString filterQmlPath(const QString &path);

  public:
    Deploy();
    bool getDeployQml() const;
    void setDeployQml(bool value);

    QString getQmlScaner() const;
    void setQmlScaner(const QString &value);
    QString getQmake() const;
    void setQmake(const QString &value);
    QString getTarget() const;
    bool setTarget(const QString &value);
    bool createRunScript();
    void deploy();
    QString getQtDir() const;
    void setQtDir(const QString &value);

    void clear();

    bool initDirs();
    void setOnlyCLibs(bool value);
    void setExtraPath(const QStringList &value);
    void setExtraPlugins(const QStringList &value);
    void setDepchLimit(int value);
};

#endif // DEPLOY_H

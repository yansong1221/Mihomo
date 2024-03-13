#pragma once
#include <SingleApplication>

class QQmlEngine;

namespace Clash::Meta {
class Application : public SingleApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char *argv[]);
    ~Application();

public:
    static void registerTypes(QQmlEngine *engine);
};
} // namespace mihomo
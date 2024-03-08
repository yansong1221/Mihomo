#pragma once
#include <SingleApplication>

namespace Clash::Meta {
class Application : public SingleApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char *argv[]);
    ~Application();
};
} // namespace mihomo
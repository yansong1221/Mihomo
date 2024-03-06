#pragma once
#include <SingleApplication>

namespace Mihomo {
class Application : public SingleApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char *argv[]);
    ~Application();
};
} // namespace mihomo
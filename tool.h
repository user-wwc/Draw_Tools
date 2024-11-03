#ifndef TOOL_H
#define TOOL_H
#include <QSqlDatabase>
#include <QSqlQuery>
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
class CTool
{
public:
    CTool();
    static void ConnectDB();



};

#endif // TOOL_H

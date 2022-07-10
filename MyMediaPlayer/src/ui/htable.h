#ifndef HTABLE_H
#define HTABLE_H
// 这是一个功能类，负责多窗口表格化
#include <map>

// 表格单元类
class HTableCell
{
public:
    HTableCell()
    {
        r1 = r2 = c1 = c2 = 0;
    }
    HTableCell(int r1, int r2, int c1, int c2)
    {
        this->r1 = r1;
        this->r2 = r2;
        this->c1 = c1;
        this->c2 = c2;
    }
    // 计算表格单元横跨块数
    int rowspan()
    {
        return r2 - r1;
    }
    // 计算表格单元纵跨块数
    int colspan()
    {
        return c2 - c1;
    }
    // 计算单元块数
    int span()
    {
        return rowspan()*colspan();
    }
    // 判断本单元中是否包含某单元
    bool contain(HTableCell cell)
    {
        if(cell.c1 >= r1 && cell.r2 <= r2 && cell.c1 >= c1 && cell.r2 <= c2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int r1, r2, c1, c2;
};

// 表格类
class HTable
{
public:
    HTable();
    void init(int row, int col);
    // 获得某个单元
    bool getTableCell(int id, HTableCell &rst);

public:
    int row;
    int col;
    std::map<int, HTableCell> m_mapCells;
};

#endif // HTABLE_H

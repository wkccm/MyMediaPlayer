#include "htable.h"
#include "hdef.h"

HTable::HTable()
{
    row = col =0;
}

// 初始化表格
void HTable::init(int row, int col)
{
    this->row = row;
    this->col = col;
    m_mapCells.clear();
    for(int r=1;r<=row;++r)
    {
        for(int c=1;c<=col;++c)
        {
            int id=(r-1)*col+c;
            // 用map做映射
            m_mapCells[id]=HTableCell(r, r+1, c, c+1);
        }
    }
}

// 判断能否按照id取到对应表格，若成功并将实参绑定在结果上
bool HTable::getTableCell(int id, HTableCell& rst)
{
    if(m_mapCells.find(id)!=m_mapCells.end())
    {
        rst = m_mapCells[id];
        return true;
    }
    return false;
}

#ifndef MY_STRBLOB_H
#define MY_STRBLOB_H

#include <vector>
#include <string>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <memory>

using namespace std;

class StrBlobPtr;   // 必须要提前声明该类，才能将其声明为StrBlob的友元类

class StrBlob
{
    friend class StrBlobPtr;
public:
    using size_type = std::vector<std::string>::size_type;
    //typedef std::vector<std::string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string>);
    StrBlob(vector<string>*);
    StrBlob(StrBlob&);
    StrBlob& operator=(StrBlob&);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    // 添加和删除元素
    void push_back(const string &t) { data->push_back(t); }
    void pop_back();
    //元素访问
    string& front();
    const string& front() const;
    string& back();
    const string& back() const;
    //提供给StrBlobPtr的接口
    StrBlobPtr begin(); // 定义StrBlobPtr之后再能实现这两个函数
    StrBlobPtr end();
    // const 版本
    StrBlobPtr begin() const;
    StrBlobPtr end() const;
private:
    // StrBlob类的位移一个数据成员
    std::shared_ptr<std::vector<std::string>> data;
    // 如果data[i]不合法，抛出一个异常
    void check(size_type, const string&) const;
};
inline
StrBlob::StrBlob() : data(make_shared<vector<string>>()) {}
inline
StrBlob::StrBlob(initializer_list<string> il) : data(make_shared<vector<string>>(il)) {}
inline
StrBlob::StrBlob(vector<string> *p) : data(p){}
inline
StrBlob::StrBlob(StrBlob& s) : data(make_shared<vector<string>>(*s.data)) {}
inline
StrBlob& StrBlob::operator=(StrBlob& rhs) { data = make_shared<vector<string>>(*rhs.data); return *this; }
inline
void StrBlob::check(size_type i, const string& msg) const { if (i >= data->size()) throw std::out_of_range(msg); }
inline
string& StrBlob::front()
{
    //若vector 为空， check会抛出异常
    check(0, "front on empty StrBlob...");
    return data->front();
}
// const版本front
inline
const string& StrBlob::front() const
{
    check(0, "front on empty StrBlob...");
    return data->front();
}
inline
string& StrBlob::back()
{
    check(0, "back on empty StrBlob...");
    return data->back();
}
// const版本back
inline
const string& StrBlob::back() const
{
    check(0, "back on empty StrBlob...");
    return data->back();
}
inline
void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob...");
    return data->pop_back();
}

class StrBlobPtr
{
    friend bool eq (const StrBlobPtr&, const StrBlobPtr&);
public:
    StrBlobPtr() : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    StrBlobPtr(const StrBlob& a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    string& deref() const;  // 解引用
    string& deref(int) const;   // 解引用的const版本
    StrBlobPtr& incr(); // 前缀递增
    StrBlobPtr& decr(); // 前缀递减
private:
    // 若检查成功，check返回一个指向vector的shared_ptr
    std::shared_ptr<std::vector<std::string>> check(size_t, const string&) const;
    // 保存一个底层weak_ptr, 意味着底层vector可能会被销毁
    std::weak_ptr<vector<string>> wptr;
    //  在数组中的当前位置
    size_t curr;
};
inline
shared_ptr<vector<string>>
StrBlobPtr::check(size_t i, const string& msg) const
{
    std::shared_ptr<vector<string>> ret = wptr.lock(); // 检查wptr指向的vector对象是否还存在
    if (!ret)
        throw std::runtime_error("Unbound StrBlobPtr...");
    if (i >= ret->size())
        throw std::out_of_range(msg);
    return ret;
}
inline string& StrBlobPtr::deref() const
{
    shared_ptr<vector<string>> ret = check(curr, "dereference past end...");
    return (*ret)[curr]; // 解引用*ret 返回的是ret指向的vector对象
}
inline string& StrBlobPtr::deref(int off) const
{
    auto ret = check(curr + off, "dereference past end...");
    return (*ret)[curr + off];
}
// 前最递增 ：返回递增后对象的引用
inline StrBlobPtr& StrBlobPtr::incr()
{
    // 若curr已经指向元素的尾后位置，就不能递增它
    check(curr, "increment past end of StrBlobPtr...");
    ++curr; // 推进当前位置
    return *this;
}
// 前缀递减 ： 返回递减后对象的引用
inline StrBlobPtr& StrBlobPtr::decr()
{
    // 若curr为0，递减它会得到一个非法下标
    --curr;
    check(-1, "decrement past begin of StrBlobPtr...");
    return *this;
}


// StrBlob的begin和end成员的定义
inline StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}
inline StrBlobPtr StrBlob::end()
{
    return StrBlobPtr(*this, this->data->size());
}
// const版本
inline StrBlobPtr StrBlob::begin() const
{
    return StrBlobPtr(*this);
}
inline StrBlobPtr StrBlob::end() const
{
    return StrBlobPtr(*this, this->data->size());
}

// StrBlobPtr的比较操作
inline bool eq(const StrBlobPtr& lhs, const StrBlobPtr& rhs)
{
    shared_ptr<vector<string>> m = lhs.wptr.lock(), n = rhs.wptr.lock();
    // 若底层vector是同一个
    if (m == n)
        // 则两个指针或者都为空， 或者都指向同一个位置
        return (!n || lhs.curr == rhs.curr);
    else
        return false;   // 若指向不同vector，则不可能相等
}
inline neq(const StrBlobPtr& lhs, const StrBlobPtr& rhs)
{
    return !eq(lhs, rhs);
}
#endif // MYSTRBLOB_H_INCLUDED

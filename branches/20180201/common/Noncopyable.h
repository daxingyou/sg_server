#pragma once

/**
 * \brief 使所有的子类禁用拷贝构造函数和赋值符号
 *
 */
class Noncopyable
{

  protected:

    /**
     * \brief 缺省构造函数
     *
     */
    Noncopyable() {};

    /**
     * \brief 缺省析构函数
     *
     */
    ~Noncopyable() {};

  private:

    /**
     * \brief 拷贝构造函数，没有实现，禁用掉了
     *
     */
    Noncopyable(const Noncopyable&);

    /**
     * \brief 赋值操作符号，没有实现，禁用掉了
     *
     */
    const Noncopyable & operator= (const Noncopyable &);

};
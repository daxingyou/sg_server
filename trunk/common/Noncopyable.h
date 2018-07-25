#pragma once

/**
 * \brief ʹ���е�������ÿ������캯���͸�ֵ����
 *
 */
class Noncopyable
{

  protected:

    /**
     * \brief ȱʡ���캯��
     *
     */
    Noncopyable() {};

    /**
     * \brief ȱʡ��������
     *
     */
    ~Noncopyable() {};

  private:

    /**
     * \brief �������캯����û��ʵ�֣����õ���
     *
     */
    Noncopyable(const Noncopyable&);

    /**
     * \brief ��ֵ�������ţ�û��ʵ�֣����õ���
     *
     */
    const Noncopyable & operator= (const Noncopyable &);

};
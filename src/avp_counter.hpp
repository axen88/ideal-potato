#ifndef _COUNTER_HPP_
#define _COUNTER_HPP_

class CTCounter
{
public:
    CTCounter(long v_cnt = 0):m_counter(v_cnt){}
    ~CTCounter();
    
    long Get(void)
    {
        return m_counter;
    }
    
    long Set(long v_cnt)
    {
        return ::InterlockedExchange(&m_counter, v_cnt);
    }
    
    long Inc(void)
    {
        return ::InterlockedIncrement(&m_counter);
    }
    
    long Dec(void)
    {
        return ::InterlockedDecrement(&m_counter);
    }
    
    long operator = (int v_cnt)
    {
        Set(v_cnt);
        return m_counter;
    }
    
private:
    long m_counter;
}; /* End of CTCounter */


#endif /* End of _COUNTER_HPP_ */

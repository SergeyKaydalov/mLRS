#pragma once
#include <functional>
#include "../../common_types.h"

#ifndef UART_ENUMS
#define UART_ENUMS

typedef enum {
  XUART_PARITY_NO = 0,
  XUART_PARITY_EVEN = 1,
  XUART_PARITY_ODD = 2,
  XUART_PARITY_MAKEITU32 = UINT32_MAX,
} UARTPARITYENUM;

typedef enum {
  UART_STOPBIT_1 = 1,
  UART_STOPBIT_2 = 2,
  UART_STOPBIT_MAKEITU32 = UINT32_MAX,
} UARTSTOPBITENUM;

typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;
#endif

template <int RX_BUF_NOB = 256, int TX_BUF_NOB = 256, int UART_BAUD = 115200>
class tLinuxSerial_ll
{
  char     ull_txbuf[TX_BUF_NOB];
  uint16_t ull_txwritepos {0}; // pos at which the last byte was stored
  uint16_t ull_txreadpos {0}; // pos at which the next byte is to be fetched

  char ull_rxbuf[RX_BUF_NOB];
  uint16_t ull_rxwritepos {0}; // pos at which the last byte was stored
  uint16_t ull_rxreadpos {0}; // pos at which the next byte is to be fetched

  uint32_t ull_errorcnt_rxnoise {0};
  uint32_t ull_errorcnt_rxframe {0};
  uint32_t ull_errorcnt_rxoverrun {0};
  uint32_t ull_enabled {false};

public:
    typedef std::function<void (uint8_t sym)> tRxCallback;
    typedef std::function<void (void)>        tTxCallback;

protected:
    tRxCallback ull_rx_cb;
    tTxCallback ull_tx_cb;

public:

    void ull_set_enabled(bool ena)
        { ull_enabled = ena; }

    bool ull_is_enabled() const
        { return ull_enabled; }

    uint16_t ull_putc(char c)
    {
        uint16_t next = (ull_txwritepos + 1) & (TX_BUF_NOB-1);
        if (ull_txreadpos != next)
        {
            ull_txbuf[next] = c;
            ull_txwritepos  = next;
            return 1;
        }
        return 0;
    }

    void ull_putbuf(uint8_t* buf, uint16_t len)
    {
        uint16_t i;
        for (i = 0; i < len; i++) ull_putc(buf[i]);
    }

    uint16_t ull_tx_notfull(void)
    {
        uint16_t next = (ull_txwritepos + 1) & (TX_BUF_NOB-1);
        return (ull_txreadpos != next);
    }

    uint16_t ull_tx_isempty(void)
    {
        if (ull_txwritepos == ull_txreadpos) return 1;
        return 0;
    }

    void ull_tx_flush(void)
    {
        ull_txwritepos = ull_txreadpos = 0;
    }
    //-------------------------------------------------------
    // RX routines
    //-------------------------------------------------------
    uint16_t ull_rx_available(void)
    {
        if (ull_rxwritepos == ull_rxreadpos) return 0; // fifo empty
        return 1;
    }

    uint16_t ull_rx_bytesavailable(void)
    {
        int16_t d;

        d = (int16_t)ull_rxwritepos - (int16_t)ull_rxreadpos;
        return (d < 0) ? d + RX_BUF_NOB : d;
    }

    char ull_getc(void)
    {
        while (ull_rxwritepos == ull_rxreadpos) {};
        ull_rxreadpos = (ull_rxreadpos + 1) & (RX_BUF_NOB-1);
        return ull_rxbuf[ull_rxreadpos];
    }

    char ull_lookatnextc(void)
    {
        return ull_rxbuf[(ull_rxreadpos + 1) & (RX_BUF_NOB-1)];
    }

    void ull_rx_flush(void)
    {
        ull_rxwritepos = ull_rxreadpos = 0;
    }

    void _ull_initprotocol(uint32_t baud, UARTPARITYENUM parity, UARTSTOPBITENUM stopbits)
    { }

    void ull_setprotocol(uint32_t baud, UARTPARITYENUM parity, UARTSTOPBITENUM stopbits)
    { }

    void ull_setbaudrate(uint32_t baud)
    {
        ull_setprotocol(baud, XUART_PARITY_NO, UART_STOPBIT_1);
    }

    void ull_tx_enablepin(FunctionalState flag)
    { }

    void ull_init_isroff(void)
    {
        ull_set_enabled(false);
        _ull_initprotocol(UART_BAUD, XUART_PARITY_NO, UART_STOPBIT_1);

        ull_txwritepos = ull_txreadpos = 0;
        ull_rxwritepos = ull_rxreadpos = 0;

        ull_errorcnt_rxnoise = 0;
        ull_errorcnt_rxframe = 0;
        ull_errorcnt_rxoverrun = 0;

        ull_set_enabled(true);
    }

    void ull_init(void)
    {
        ull_init_isroff();
    }

    uint8_t ull_has_systemboot(void)
        { return 0; }
};


class tLinuxSerialBase : public tSerialBase
{
    const char *tls_tty_name;
public:
    typedef std::function<void (uint8_t sym)> tRxCallback;
    typedef std::function<void (void)>        tTxCallback;
private:
    tRxCallback tls_rx_cb;
    tTxCallback tls_tx_cb;

public:
    void tlsSetCallbacks(tRxCallback rx_cb, tTxCallback tx_cb)
        {
            tls_rx_cb = rx_cb;
            tls_tx_cb = tx_cb;
        }
public:
    tLinuxSerialBase(const char *tty_name);
    void Init(void) override;
    void SetBaudRate(uint32_t baud) override;
    void putbuf(uint8_t* const buf, uint16_t len) override;
    bool available(void) override;
    char getc(void) override;
    void flush(void) override;
    uint16_t bytes_available(void) override;
};

class tLinuxDebugBase : public tSerialBase
{
public:
    tLinuxDebugBase();
    void putbuf(uint8_t* const buf, uint16_t len);
};

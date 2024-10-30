#include "../../common_types.h"

class tLinuxSerialBase : public tSerialBase
{
    const char *tls_tty_name;
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

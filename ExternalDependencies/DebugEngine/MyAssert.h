#ifndef MY_ASSERT_H
#define MY_ASSERT_H

#define Assert(exp) do { if(!(exp)) __FILE__ _DebugBreak(); } while(0);

#endif //myassert
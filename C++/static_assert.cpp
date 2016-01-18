template<bool> struct CompileTimeError {
    CompileTimeError(...);
};
template<> struct CompileTimeError<false> {};
#define STATIC_ASSERT(expr, msg) \
    { \
        class ERROR_##msg {}; \
        CompileTimeError<(expr) != 0>(ERROR_##msg()); \
    }

int main()
{
    STATIC_ASSERT(1, 1);
    STATIC_ASSERT(0, 0);
    STATIC_ASSERT(true, True);
    STATIC_ASSERT(false, False);
    return 0;
}

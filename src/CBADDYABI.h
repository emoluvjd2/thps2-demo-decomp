/*
 * Re-entrant lifecycle ABI selectors. Owner translation units retain their
 * target-proven receiver views while sharing CBaddy's target labels.
 */
#if defined(CBADDY_DECLARE_CONSTRUCTOR)
#if !defined(CBADDY_CONSTRUCTOR_NAMESPACE) || \
    !defined(CBADDY_CONSTRUCTOR_RECEIVER)
#error CBADDY_DECLARE_CONSTRUCTOR requires a namespace and receiver type
#endif
namespace CBADDY_CONSTRUCTOR_NAMESPACE
{
/** Construct a baddy through its target default-constructor entry. */
extern "C" CBADDY_CONSTRUCTOR_RECEIVER *__6CBaddy(
    CBADDY_CONSTRUCTOR_RECEIVER *baddy);
}
#undef CBADDY_CONSTRUCTOR_RECEIVER
#undef CBADDY_CONSTRUCTOR_NAMESPACE
#undef CBADDY_DECLARE_CONSTRUCTOR
#endif

#if defined(CBADDY_DECLARE_DESTRUCTOR)
#if !defined(CBADDY_DESTRUCTOR_NAME) || \
    !defined(CBADDY_DESTRUCTOR_RECEIVER)
#error CBADDY_DECLARE_DESTRUCTOR requires a name and receiver type
#endif
void CBADDY_DESTRUCTOR_NAME(
    CBADDY_DESTRUCTOR_RECEIVER *baddy, int inCharge)
    asm("_._6CBaddy");
#undef CBADDY_DESTRUCTOR_RECEIVER
#undef CBADDY_DESTRUCTOR_NAME
#undef CBADDY_DECLARE_DESTRUCTOR
#endif

#if defined(CBADDY_DECLARE_SCRIPT_ONLY_DESTRUCTOR)
#if !defined(CBADDY_SCRIPT_ONLY_DESTRUCTOR_NAME) || \
    !defined(CBADDY_SCRIPT_ONLY_DESTRUCTOR_RECEIVER)
#error CBADDY_DECLARE_SCRIPT_ONLY_DESTRUCTOR requires a name and receiver type
#endif
/** Addressable destructor entry used by the script-only baddy vtable. */
void CBADDY_SCRIPT_ONLY_DESTRUCTOR_NAME(
    CBADDY_SCRIPT_ONLY_DESTRUCTOR_RECEIVER *baddy, int inCharge)
    asm("_._16CScriptOnlyBaddy");
#undef CBADDY_SCRIPT_ONLY_DESTRUCTOR_RECEIVER
#undef CBADDY_SCRIPT_ONLY_DESTRUCTOR_NAME
#undef CBADDY_DECLARE_SCRIPT_ONLY_DESTRUCTOR
#endif

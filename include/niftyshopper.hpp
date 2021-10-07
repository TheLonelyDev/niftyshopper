#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

#include <atomicassets.hpp>

CONTRACT niftyshopper : public eosio::contract
{
public:
    using eosio::contract::contract;

    void receive_token_transfer(
        eosio::name from,
        eosio::name to,
        eosio::asset quantity,
        std::string memo);

    [[eosio::action]] void setstore(
        uint64_t template_id,

        eosio::name token_contract,
        eosio::asset buy_price,
        bool burn_token);

    [[eosio::action]] void rmstore(
        uint64_t template_id);

    [[eosio::action]] void init();
    [[eosio::action]] void destruct();
    [[eosio::action]] void maintenance(bool maintenance);

private:
    struct [[eosio::table("config")]] _config_entity
    {
        bool maintenance = true;
    };
    typedef eosio::singleton<eosio::name("config"), _config_entity> _config;

    struct [[eosio::table("store")]] _store_entity
    {
        uint64_t template_id;

        eosio::name token_contract;
        eosio::asset buy_price;

        bool burn_token;

        uint64_t primary_key() const { return template_id; }
    };
    typedef eosio::multi_index<eosio::name("store"), _store_entity> _store;

    _config get_config()
    {
        return _config(get_self(), get_self().value);
    }

    _store get_store()
    {
        return _store(get_self(), get_self().value);
    }

    void maintenace_check();
};

extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
    if (code == receiver)
    {
        switch (action)
        {
            EOSIO_DISPATCH_HELPER(niftyshopper, (setstore)(rmstore)(init)(destruct)(maintenance))
        }
    }
    else if (action == eosio::name("transfer").value)
    {
        eosio::execute_action(eosio::name(receiver), eosio::name(code), &niftyshopper::receive_token_transfer);
    }
}
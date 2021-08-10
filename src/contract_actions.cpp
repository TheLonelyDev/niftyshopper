/*
    Adds an entry to the allowed templates that can be bought from the smart contract.

    @param {bool} template_id - the template id of the asset that can be bought
    @param {bool} token_contract - the contract of the FT, eg eosio.token
    @param {bool} buy_price - the buy price eg 19 WAX of the NFT

    @auth self
*/
[[eosio::action]] void niftyshopper::setstore(
    uint64_t template_id,

    eosio::name token_contract,
    eosio::asset buy_price)
{
    require_auth(get_self());

    auto stores = get_store();
    auto store = stores.find(template_id);

    auto object = (_store_entity){
        .template_id = template_id,
        .token_contract = token_contract,
        .buy_price = buy_price};

    if (store == stores.end())
    {
        stores.emplace(get_self(), [&object = object](auto &row)
                       { row = object; });
    }
    else
    {
        stores.modify(store, get_self(), [&object = object](auto &row)
                      { row = object; });
    }
}

/*
    Deletes the entry from the store, preventing it to be bought.

    @param {bool} template_id - the template id of the asset that can be bought

    @auth self
*/
[[eosio::action]] void niftyshopper::rmstore(
    uint64_t template_id)
{
    require_auth(get_self());

    auto stores = get_store();
    auto store = stores.require_find(template_id, "Could not find entry");

    stores.erase(store);
}

/*
    Initialize the contract configuration singleton, removes the previous configuration first.

    @auth self
*/
[[eosio::action]] void niftyshopper::init()
{
    require_auth(get_self());
    get_config().remove();

    get_config().set(_config_entity{}, get_self());
}

/*
    Delete the contract configuration singleton.

    @auth self
*/
[[eosio::action]] void niftyshopper::destruct()
{
    require_auth(get_self());
    get_config().remove();
}

/*
    Set the contract in maintenance mode.

    Maintenance mode prevents users from doing any actions.

    @param {bool} maintenance - maintenance toggle

    @auth self
*/
[[eosio::action]] void niftyshopper::maintenance(bool maintenance)
{
    require_auth(get_self());

    auto config = get_config();
    auto new_config = config.get();

    new_config.maintenance = maintenance;

    config.set(new_config, get_self());
}
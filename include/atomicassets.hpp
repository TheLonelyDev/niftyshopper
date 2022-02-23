#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

namespace atomicassets
{
    static constexpr eosio::name ATOMICASSETS_ACCOUNT = eosio::name("atomicassets");

    // Scope: owner
    struct assets_s
    {
        uint64_t asset_id;
        eosio::name collection_name;
        eosio::name schema_name;
        int32_t template_id;
        eosio::name ram_payer;
        std::vector<eosio::asset> backed_tokens;
        std::vector<uint8_t> immutable_serialized_data;
        std::vector<uint8_t> mutable_serialized_data;

        uint64_t primary_key() const { return asset_id; };
    };

    typedef eosio::multi_index<eosio::name("assets"), assets_s> assets_t;

    assets_t get_assets(eosio::name acc)
    {
        return assets_t(ATOMICASSETS_ACCOUNT, acc.value);
    }
};
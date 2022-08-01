cmake_minimum_required(VERSION 3.22)
project(dbm_experimenting)

set(CMAKE_CXX_STANDARD 17)

set(headers
        model.hpp
        detail/common.hpp
        detail/model_item_base.hpp
        detail/model_item.hpp
        )

set(sources
        main.cpp)

add_executable(dbm_experimenting ${sources} ${headers})

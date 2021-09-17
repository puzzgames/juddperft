//
// Created by andrzej on 9/17/21.
//

#ifndef JUDDPERFT_STRUTILS_H
#define JUDDPERFT_STRUTILS_H

#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

std::string ms_to_smh(double ms) {
    double seconds = ms/1000;
    if (seconds<60) {
        return string_format("%.0f s",seconds);
    } else if (seconds<3600) {
        return string_format("%.1f m",seconds/60);
    } else return string_format("%%.1f h",seconds/3600);
}

#endif //JUDDPERFT_STRUTILS_H

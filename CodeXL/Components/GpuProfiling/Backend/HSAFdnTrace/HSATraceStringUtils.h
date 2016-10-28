//==============================================================================
// Copyright (c) 2015 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  This file contains functions to format HSA Trace string output
//==============================================================================

#ifndef _HSA_TRACE_STRING_UTILS_H_
#define _HSA_TRACE_STRING_UTILS_H_

#include <sstream>
#include <string>
#include <vector>
#include <AutoGenerated/HSARTModuleDecls.h>
#include "HSAFunctionDefs.h"
#include "Defs.h"
#include "StringUtils.h"

namespace HSATraceStringUtils
{
static const char* s_pListStart = "{";      ///< start of a list of items
static const char* s_pListEnd = "}";        ///< end of a list of items
static const char* s_pStructStart = "{";    ///< start of a structure
static const char* s_pStructEnd = "}";      ///< end of a structure
static const char* s_pPtrDeRefStart = "[";  ///< start of a dereferenced pointer
static const char* s_pPtrDeRefEnd = "]";    ///< end of a dereferenced pointer

/// Pointer to a function to use to get the display name for an API
/// \param type the API whose name is needed
/// \param[out] apiDisplayName the display name of the specified API
/// \return true if the display name is returned for the specified API
extern bool (*pGetApiDisplayName)(HSA_API_Type type, std::string& apiDisplayName);

/// General template function to surround an input with two other strings
/// \param input the input string
/// \param pSurroundBegin the string to put at the beginning of the input
/// \param pSurroundEnd the string to put at the end of the input
/// \return the string containing the surrounded input
template<typename T>
std::string SurroundWith(T input, const char* pSurroundBegin, const char* pSurroundEnd)
{
    std::ostringstream ss;
    ss << pSurroundBegin;
    ss << input;
    ss << pSurroundEnd;

    return ss.str();
}

/// Template function to surround an input with pointer deref strings
/// \param input the input string
/// \return the string containing the surrounded input
template<typename T>
std::string SurroundWithDeRef(T input)
{
    return SurroundWith(input, s_pPtrDeRefStart, s_pPtrDeRefEnd);
}

/// Template function to surround an input with structure strings strings
/// \param input the input string
/// \return the string containing the surrounded input
template<typename T>
std::string SurroundWithStruct(T input)
{
    return SurroundWith(input, s_pStructStart, s_pStructEnd);
}

/// Template function to surround an input with list strings
/// \param input the input string
/// \return the string containing the surrounded input
template<typename T>
std::string SurroundWithList(T input)
{
    return SurroundWith(input, s_pListStart, s_pListEnd);
}

/// Gets the string representation of the HSA_API_TYPE
/// \param type the input HSA_API_TYPE
/// \return the string representation of the HSA_API_TYPE
std::string GetHSAAPINameString(HSA_API_Type type);

/// Add quotes to input string and optionally truncate it at most 60 chars
/// \param src input string
/// \param truncate flag indicating whether the output should be limited to 60 chars
/// \param surroundWithDeref flag indicating whether to surround the string with brackets to indicate the value is  a dereferenced value
/// \return string representation of the input
std::string GetStringString(const char* src, bool truncate = true, bool surroundWithDeref = true);

/// Add quotes to input string and optionally trucate it at most 60 chars. This overload will output a string only if input pointer is non-null
/// \param src input string pointer
/// \param srcString input string
/// \param truncate flag indicating whether the output should be limited to 60 chars
/// \param surroundWithDeref flag indicating whether to surround the string with brackets to indicate the value is  a dereferenced value
/// \return string representation of the input
std::string GetStringString(const char* src, std::string& srcString, bool truncate = true, bool surroundWithDeref = true);

/// Template fnuction to output up to three items in an array (more will be indicated with an elipsis)
/// \param input the input array
/// \param the number of items in the array
/// \param formatfunc the string format fnuction to call for each memeber of the array
/// \return string representation of the input
template<typename T>
std::string GetArrayString(T* input, size_t num_items, std::string(*formatFunc)(T))
{
    static const size_t MAX_ITEMS_TO_OUTPUT = 3;

    if (NULL == input || NULL == formatFunc)
    {
        return "";
    }

    std::ostringstream ss;
    bool truncated = false;
    size_t numItemsToOutput = num_items;

    if (numItemsToOutput > MAX_ITEMS_TO_OUTPUT)
    {
        numItemsToOutput = MAX_ITEMS_TO_OUTPUT;
        truncated = true;
    }

    for (size_t i = 0; i < numItemsToOutput; ++i)
    {
        ss << formatFunc(input[i]);

        if (i < numItemsToOutput - 1 || truncated)
        {
            ss << ",";
        }
    }

    if (truncated)
    {
        ss << "...";
    }

    return SurroundWithList(ss.str());
}

/// Gets the string representation of bool
/// \param input the input bool
/// \return the string representation of the bool
std::string Get_bool_String(bool input);

/// Gets the string representation of a bool pointer
/// \param input the input bool pointer
/// \param de-referenced value of the input bool
/// \return the string representation of the bool pointer
std::string Get_bool_Ptr_String(bool* pInput, bool input);

/// Gets the string representation of a uint8_t
/// This method is needed so uint8_t values aren't displayed as char data
/// \param input the input uint8_t
/// \return the string representation of the uint8_t
std::string Get_uint8_t_String(uint8_t input);

/// Gets the size of the underlying data for the specified agent attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified agent attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_agent_get_info_AttributeSize(hsa_agent_info_t attribute);

/// Gets the string representation of the specified agent attribute data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_agent_get_info call
/// \return the string representation of the specified agent attribute data
std::string Get_hsa_agent_get_info_AttributeString(void* value, hsa_agent_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified agent attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified agent attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_amd_agent_get_info_AttributeSize(hsa_amd_agent_info_t attribute);

/// Gets the string representation of the specified agent attribute data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_agent_get_info call
/// \return the string representation of the specified agent attribute data
std::string Get_hsa_amd_agent_get_info_AttributeString(void* value, hsa_amd_agent_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified system attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified agent attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_system_get_info_AttributeSize(hsa_system_info_t attribute);

/// Gets the string representation of the specified system attribute data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_system_get_info call
/// \return the string representation of the specified agent attribute data
std::string Get_hsa_system_get_info_AttributeString(void* value, hsa_system_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified region attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified region attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_region_get_info_AttributeSize(hsa_region_info_t attribute);

/// Gets the string representation of the specified region data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_region_get_info call
/// \return the string representation of the specified agent attribute data
std::string Get_hsa_region_get_info_AttributeString(void* value, hsa_region_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified isa attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified isa attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_isa_get_info_AttributeSize(hsa_isa_info_t attribute);

/// Gets the string representation of the specified isa data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_isa_get_info call
/// \return the string representation of the specified agent attribute data
std::string Get_hsa_isa_get_info_AttributeString(void* value, hsa_isa_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified code object attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified code object attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_code_object_get_info_AttributeSize(hsa_code_object_info_t attribute);

/// Gets the string representation of the specified code object data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_code_object_get_info call
/// \return the string representation of the specified code object attribute data
std::string Get_hsa_code_object_get_info_AttributeString(void* value, hsa_code_object_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified code symbol attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified code symbol attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_code_symbol_get_info_AttributeSize(hsa_code_symbol_info_t attribute);

/// Gets the string representation of the specified code symbol data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_code_symbol_get_info call
/// \return the string representation of the specified code symbol attribute data
std::string Get_hsa_code_symbol_get_info_AttributeString(void* value, hsa_code_symbol_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified executable attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified executable attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_executable_get_info_AttributeSize(hsa_executable_info_t attribute);

/// Gets the string representation of the specified executable data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_executable_get_info call
/// \return the string representation of the specified agent executable data
std::string Get_hsa_executable_get_info_AttributeString(void* value, hsa_executable_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified executable symbol attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified executable symbol attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_executable_symbol_get_info_AttributeSize(hsa_executable_symbol_info_t attribute);

/// Gets the string representation of the specified executable symbol data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_executable_symbol_get_info call
/// \return the string representation of the specified agent executable symbol data
std::string Get_hsa_executable_symbol_get_info_AttributeString(void* value, hsa_executable_symbol_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified program attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified program attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_ext_program_get_info_AttributeSize(hsa_ext_program_info_t attribute);

/// Gets the string representation of the specified program data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_ext_program_get_info call
/// \return the string representation of the specified program data
std::string Get_hsa_ext_program_get_info_AttributeString(void* value, hsa_ext_program_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified memory pool attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified memory pool attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_amd_memory_pool_get_info_AttributeSize(hsa_amd_memory_pool_info_t attribute);

/// Gets the string representation of the specified memory pool data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_amd_memory_pool_get_info call
/// \return the string representation of the specified memory pool data
std::string Get_hsa_amd_memory_pool_get_info_AttributeString(void* value, hsa_amd_memory_pool_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified agent memory pool attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified agent memory pool attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_amd_agent_memory_pool_get_info_AttributeSize(hsa_amd_agent_memory_pool_info_t attribute);

/// Gets the string representation of the specified agent memory pool data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_amd_agent_memory_pool_get_info call
/// \return the string representation of the specified agent memory pool data
std::string Get_hsa_amd_agent_memory_pool_get_info_AttributeString(void* value, hsa_amd_agent_memory_pool_info_t attribute, hsa_status_t retVal);

#ifdef FUTURE_ROCR_VERSION

/// Gets the size of the underlying data for the specified cache attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified cache attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_cache_get_info_AttributeSize(hsa_cache_info_t attribute);

/// Gets the string representation of the specified cache data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_cache_get_info call
/// \return the string representation of the specified cache data
std::string Get_hsa_cache_get_info_AttributeString(void* value, hsa_cache_info_t attribute, hsa_status_t retVal);

/// Gets the size of the underlying data for the specified wavefront attribute
/// \param attribute the attribute whose size is needed
/// \return the size of the underlying data for the specified wavefront attribute (0 for unsupported/unknown attributes)
unsigned int Get_hsa_wavefront_get_info_AttributeSize(hsa_wavefront_info_t attribute);

/// Gets the string representation of the specified wavefront data
/// \param value the value whose string representation is needed
/// \param attribute the attribute type
/// \param the return value of the hsa_wavefront_get_info call
/// \return the string representation of the specified wavefront data
std::string Get_hsa_wavefront_get_info_AttributeString(void* value, hsa_wavefront_info_t attribute, hsa_status_t retVal);
#endif

}

#endif //_HSA_TRACE_STRING_UTILS_H_

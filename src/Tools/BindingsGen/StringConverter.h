// ------------------------------------------------------------------------------------------- //
// CppSharp C/CLI helpers
//
// String marshaling code adapted from:
//    http://blog.nuclex-games.com/mono-dotnet/cxx-cli-string-marshaling
//
// Licensed under the MIT license
// ------------------------------------------------------------------------------------------- //
#pragma once

#include "Core/Containers/Str.h"
#include <vcclr.h>

namespace StringMarshaller
{
      static System::String^ marshalString(const String& str)
      {
        size_t byteCount = str.Length();

        if(byteCount == 0) return System::String::Empty;

        // Copy the C string contents into a managed array of bytes
        array<unsigned char> ^bytes = gcnew array<unsigned char>(byteCount);
        { 
            pin_ptr<unsigned char> pinnedBytes = &bytes[0];
            memcpy(pinnedBytes, str.CString(), byteCount);
        }

        return System::Text::Encoding::Default->GetString(bytes);
      }      
      
      static System::String^ marshalString(const UTF8String& str)
      {
        size_t byteCount = str.ByteLength();

        if(byteCount == 0) return System::String::Empty;

        // Copy the C string contents into a managed array of bytes
        array<unsigned char> ^bytes = gcnew array<unsigned char>(byteCount);
        { 
            pin_ptr<unsigned char> pinnedBytes = &bytes[0];
            memcpy(pinnedBytes, str.CString(), byteCount);
        }

        return System::Text::Encoding::UTF8->GetString(bytes);
      }      
      
      static System::String^ marshalString(const WString& str)
      {
        size_t byteCount = str.Bytes();

        if(byteCount == 0) return System::String::Empty;

        // Copy the C string contents into a managed array of bytes
        array<unsigned char> ^bytes = gcnew array<unsigned char>(byteCount);
        { 
            pin_ptr<unsigned char> pinnedBytes = &bytes[0];
            memcpy(pinnedBytes, str.CString(), byteCount);
        }

        return System::Text::Encoding::Unicode->GetString(bytes);
      }

    
      static String marshalString(System::String ^string) 
      {
        // First, we use .NET's encoding classes to convert the string into a byte array
        array<unsigned char> ^bytes = System::Text::Encoding::Default->GetBytes(string);

        if(bytes->Length == 0) return String();

        // Then we construct our native string from that byte array
        pin_ptr<unsigned char> pinnedBytes(&bytes[0]);
        return String(
          reinterpret_cast<char *>(static_cast<unsigned char *>(pinnedBytes)),
          bytes->Length);
      }

      static WString marshalWString(System::String ^string) 
      {
        // First, we use .NET's encoding classes to convert the string into a byte array
        array<unsigned char> ^bytes = System::Text::Encoding::Unicode->GetBytes(string);

        if(bytes->Length == 0) return WString();

        // Then we construct our native string from that byte array
        pin_ptr<unsigned char> pinnedBytes(&bytes[0]);
        return WString(
          reinterpret_cast<wchar_t *>(static_cast<unsigned char *>(pinnedBytes)));
      }

      static UTF8String marshalUTF8String(System::String ^string) 
      {
        // First, we use .NET's encoding classes to convert the string into a byte array
        array<unsigned char> ^bytes = System::Text::Encoding::UTF8->GetBytes(string);

        if(bytes->Length == 0) return UTF8String();

        // Then we construct our native string from that byte array
        pin_ptr<unsigned char> pinnedBytes(&bytes[0]);
        return UTF8String(
          reinterpret_cast<char *>(static_cast<unsigned char *>(pinnedBytes)),
          bytes->Length, true);
      }
}

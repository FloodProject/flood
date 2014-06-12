
#pragma once

#using <System.core.dll>

#include "Core/Containers/HashSet.h"
#include "Core/Containers/HashMap.h"
#include "Core/Containers/Vector.h"

namespace ContainerMarshaller
{
      template<typename T, typename U>
      static System::Collections::Generic::Dictionary<T,U>^ marshalDictionary(const HashMap<T, U>& map)
      {
        System::Collections::Generic::Dictionary<T,U>^ dictionary = gcnew System::Collections::Generic::Dictionary<T,U>();
        
        for(auto key : map.Keys())
            dictionary.Add(key, map[key]);

        return dictionary;
      }

      template<typename T>

      static System::Collections::Generic::HashSet<T>^ marshalHashSet(const HashSet<T>& set)
      {
        System::Collections::Generic::HashSet<T>^ hashSet = gcnew System::Collections::Generic::HashSet<T>();
        
        for(auto it = set.Begin(); it != set.End(); ++it)
            hashSet.Add(*it);

        return hashSet;
      }

      template<typename T, typename N>
      static System::Collections::Generic::List<T>^ marshalList(const Vector<T>& vec)
      {
        System::Collections::Generic::List<T>^ list = gcnew System::Collections::Generic::List<T>();
        
        for(auto it = vec.Begin(); it != vec.End(); ++it)
            list.Add(*it);

        return list;
      }

      template<typename T, typename U>
      static HashMap<T, U> marshalHashMap(System::Collections::Generic::Dictionary<T,U>^ dictionary)
      {
        HashMap<T, U> map;
        
        for(auto key : dictionary.Keys)
            map[key] = dictionary[key];

        return map;
      }

      template<typename T>
      static HashSet<T> marshalHashSet(System::Collections::Generic::HashSet<T>^ hashSet)
      {
        HashSet<T> set;
        
        for(T el : hashSet)
            set.Insert(el);

        return set;
      }

      template<typename T>
      static Vector<T> marshalVector(System::Collections::Generic::List<T>^ list)
      {
        Vector<T> vec;
        
        for(T el : list)
            vec.Push(el);

        return vec;
      }
      
}

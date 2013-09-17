	/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Containers/HashMap.h"
#include "Core/Containers/HashSet.h"
#include "Core/Containers/Str.h"
#include <UnitTest++.h>

SUITE(Core)
{
	TEST(HashMap)
	{
		HashMap<int, String> map;
		map[1] = "foo";
		CHECK_EQUAL(map.Size(), 1);
		CHECK_EQUAL("foo", map[1]);

		map[2] = "foo";
		map[3] = "bar";
		map[4] = "baz";

		CHECK_EQUAL(map.Size(), 4);
		CHECK(map[1] == map[2]);
		CHECK(map.Contains(1));
		CHECK(!map.Contains(0));

		HashMap<int, String> map2(map);
		HashMap<int, String> map3 = map2;
		
		CHECK(map == map2);
		CHECK(map == map3);
		
		map3 += MakePair<int, String>(5, "foo");
		CHECK(map != map3);
		CHECK_EQUAL(map3.Size(), 5);
		CHECK_EQUAL(map3[1], "foo");
		CHECK_EQUAL(map3[5], "foo");

		map3.Erase(5);
		CHECK(!map3.Contains(5));
		map3.Clear();
		CHECK_EQUAL(map3.Size(), 0);
		CHECK_EQUAL(map3[0], "");

		String str = "";
		for(int i = 0; i < 100; ++i)
		{	
			map3[i] = str += "s";
		}
		str = "";

		for(int i = 0; i < 100; ++i)
		{	
			CHECK_EQUAL(map3[i],  str += "s");
		}

		Vector<int> keys = map3.Keys();

		for(int i = 0; i < 100; ++i)
		{	
			CHECK_EQUAL(keys[i],  i);
		}

		HashMap<int, double> movemap1, movemap2;
		movemap1[1] = 2.3;
		movemap1[10] = 5.4;
		movemap2 = std::move(movemap1);

		CHECK(movemap1.Size() == 0);
		CHECK_EQUAL(movemap2.Size(), 2);
		CHECK_EQUAL(movemap2[1], 2.3);
		CHECK_EQUAL(movemap2[10], 5.4);

		HashMap<int, double> movemap3(std::move(movemap2));
		CHECK(movemap2.Size() == 0);
		CHECK_EQUAL(movemap3.Size(), 2);
		CHECK_EQUAL(movemap3[1], 2.3);
		CHECK_EQUAL(movemap3[10], 5.4);
	}

	TEST(Vector)
	{
		Vector<String> vec, vec2, vec3;
		CHECK_EQUAL(vec.Size(), 0);

		vec.Push("str1");
		vec.Push("str2");
		vec.Push("str3");

		CHECK_EQUAL(vec.Size(), 3);
		CHECK_EQUAL(vec[0], "str1");
		CHECK_EQUAL(vec[1], "str2");
		CHECK_EQUAL(vec[2], "str3");
		CHECK(vec.Contains("str1"));
		CHECK(!vec.Contains("str0"));
		auto it = vec.Find("str2");
		CHECK_EQUAL(*it, "str2");
		CHECK_EQUAL(*++it, "str3");
		
		vec.Remove("str2");
		CHECK_EQUAL(vec.Size(), 2);
		CHECK_EQUAL(vec[0], "str1");
		CHECK_EQUAL(vec[1], "str3");
		
		vec.Pop();
		vec.Pop();
		CHECK_EQUAL(vec.Size(), 0);
		CHECK(vec.Empty());

		vec.Push("str1");
		vec.Push("str2");
		vec.Push("str3");

		vec2.Push("str1");
		vec2.Push("str2");
		vec2.Push("str3");

		CHECK(vec == vec2);
		vec2 = vec;
		CHECK(vec == vec2);

		vec3.Push("str4");
		vec2[1] = "str5";
		CHECK(vec != vec3);
		CHECK(vec != vec2);
		vec += vec2;
		CHECK_EQUAL(vec.Size(), 6);
		CHECK_EQUAL(vec[0], "str1");
		CHECK_EQUAL(vec[1], "str2");
		CHECK_EQUAL(vec[2], "str3");
		CHECK_EQUAL(vec[3], "str1");
		CHECK_EQUAL(vec[4], "str5");
		CHECK_EQUAL(vec[5], "str3");
		CHECK_EQUAL(vec.Back(), "str3");
		CHECK_EQUAL(vec.Front(), "str1");

		vec += vec3;
		CHECK_EQUAL(vec.Size(), 7);
		CHECK_EQUAL(vec.At(6), "str4");

		vec += "str6";

		CHECK_EQUAL(vec.Size(), 8);
		CHECK_EQUAL(vec[7], "str6");

		vec = vec2 + "str4";
		CHECK_EQUAL(vec.Size(), 4);
		CHECK_EQUAL(vec[3], "str4");

		Vector<String> mvec(std::move(vec)), mvec2(vec2 + "str4"), vec4(mvec);
		CHECK_EQUAL(mvec.Size(), 4);
		CHECK_EQUAL(vec.Size(), 0);
		CHECK(mvec == mvec2);
		CHECK(vec4 == mvec2);
		vec4 = std::move(mvec2);
		CHECK_EQUAL(mvec2.Size(), 0);
		CHECK(mvec != mvec2);
		CHECK(mvec == vec4);


		Vector<String, 3> svec, svec2, svec3;
		CHECK_EQUAL(svec.Size(), 0);

		svec.Push("str1");
		svec.Push("str2");
		svec.Push("str3");

		CHECK_EQUAL(svec.Size(), 3);
		CHECK_EQUAL(svec[0], "str1");
		CHECK_EQUAL(svec[1], "str2");
		CHECK_EQUAL(svec[2], "str3");
		CHECK(svec.Contains("str1"));
		CHECK(!svec.Contains("str0"));
		it = svec.Find("str2");
		CHECK_EQUAL(*it, "str2");
		CHECK_EQUAL(*++it, "str3");
		
		svec.Remove("str2");
		CHECK_EQUAL(svec.Size(), 2);
		CHECK_EQUAL(svec[0], "str1");
		CHECK_EQUAL(svec[1], "str3");
		
		svec.Pop();
		svec.Pop();
		CHECK_EQUAL(svec.Size(), 0);
		CHECK(svec.Empty());

		svec.Push("str1");
		svec.Push("str2");
		svec.Push("str3");

		svec2.Push("str1");
		svec2.Push("str2");
		svec2.Push("str3");

		CHECK(svec == svec2);
		svec2 = svec;
		CHECK(svec == svec2);

		svec3.Push("str4");
		svec2[1] = "str5";
		CHECK(svec != svec3);
		CHECK(svec != svec2);
		svec += svec2;
		CHECK_EQUAL(svec.Size(), 6);
		CHECK_EQUAL(svec[0], "str1");
		CHECK_EQUAL(svec[1], "str2");
		CHECK_EQUAL(svec[2], "str3");
		CHECK_EQUAL(svec[3], "str1");
		CHECK_EQUAL(svec[4], "str5");
		CHECK_EQUAL(svec[5], "str3");
		CHECK_EQUAL(svec.Back(), "str3");
		CHECK_EQUAL(svec.Front(), "str1");

		svec += svec3;
		CHECK_EQUAL(svec.Size(), 7);
		CHECK_EQUAL(svec.At(6), "str4");

		svec += "str6";

		CHECK_EQUAL(svec.Size(), 8);
		CHECK_EQUAL(svec[7], "str6");

		svec = svec2 + "str4";
		CHECK_EQUAL(svec.Size(), 4);
		CHECK_EQUAL(svec[3], "str4");

		Vector<String,3> msvec(std::move(svec)), msvec2(svec2 + "str4"), svec4(msvec);
		CHECK_EQUAL(msvec.Size(), 4);
		CHECK_EQUAL(svec.Size(), 0);
		CHECK(msvec == msvec2);
		CHECK(svec4 == msvec2);
		svec4 = std::move(msvec2);
		CHECK_EQUAL(msvec2.Size(), 0);
		CHECK(msvec != msvec2);
		CHECK(msvec == svec4);
	}

	TEST(HashSet)
	{
		HashSet<String> set, set2;
		set.Insert("str1");
		set.Insert("str2");
		set.Insert("str3");
		CHECK_EQUAL(set.Size(), 3);
		set.Sort();
		auto it = set.Begin();
		CHECK_EQUAL(*it++, "str1");
		CHECK_EQUAL(*it++, "str2");
		CHECK_EQUAL(*it, "str3");
		CHECK(set.Contains("str1"));
		CHECK(!set.Contains("str4"));
		CHECK(!set.Empty());
		set2 = set;
		CHECK(set2 == set);
		set.Clear();
		CHECK(set.Empty());
		CHECK_EQUAL(set.Size(), 0);
		CHECK(set != set2);
		set.Insert("str3");
		set.Insert("str1");
		set.Insert("str2");
		CHECK(set == set2);
		CHECK(set.Contains("str3"));
		set.Erase("str3");
		CHECK(!set.Contains("str3"));
		CHECK_EQUAL(set.Size(), 2);
		set.Insert("str4");
		CHECK(set != set2);
		HashSet<String> set3(set);
		CHECK_EQUAL(set3.Size(), 3);
		CHECK(set == set3);
		set3 += set2;
		CHECK_EQUAL(set3.Size(), 4);
		set3.Sort();
		it = set3.Begin();
		CHECK_EQUAL(*it++, "str1");
		CHECK_EQUAL(*it++, "str2");
		CHECK_EQUAL(*it++, "str3");
		CHECK_EQUAL(*it, "str4");
		it = set3.Find("str3");
		CHECK_EQUAL(*it++, "str3");
		CHECK_EQUAL(*it++, "str4");
		CHECK_EQUAL(set3.Front(), "str1");
		CHECK_EQUAL(set3.Back(), "str4");
		CHECK(it == set3.End());
		it = set3.Find("str5");
		CHECK(it == set3.End());
		
		set2 = set;
		HashSet<String> mset(std::move(set)), mset2;
		CHECK(set2 == mset);
		CHECK(set.Empty());
		CHECK_EQUAL(mset.Size(), 3);
		set2  = set3;
		mset2=std::move(set3);
		CHECK(set2 == mset2);
		CHECK_EQUAL(mset2.Size(), 4);
		CHECK(set3.Empty());
	}

	TEST(Str)
	{
		int i = -2147483648;
		short s = -32768;
		long l = 2147483647;
		long long ll = -9223372036854775808;
		unsigned u = 4294967295;
		unsigned short us = 65535;
		unsigned long ul = 4294967295;
		unsigned long long ull = 18446744073709551615;
		float f = FLT_MAX;
		double d = DBL_MAX;

		String str1('a', 4);
		String str2('a');
		String str3(true);
		String str4(i);
		String str5(s);
		String str6(l);
		String str7(ll);
		String str8(u);
		String str9(us);
		String str10(ul);
		String str11(ull);
		String str12(f);
		String str13(d);


		CHECK_EQUAL(str1, "aaaa");
		CHECK_EQUAL(str2, "a");
		CHECK_EQUAL(str3, "true");
		CHECK_EQUAL(str4, "-2147483648");
		CHECK_EQUAL(str5, "-32768");
		CHECK_EQUAL(str6, "2147483647");
		CHECK_EQUAL(str7, "-9223372036854775808");
		CHECK_EQUAL(str8, "4294967295");
		CHECK_EQUAL(str9, "65535");
		CHECK_EQUAL(str10, "4294967295");
		CHECK_EQUAL(str11, "18446744073709551615");
		CHECK_EQUAL(str12, "3.40282e+038");
		CHECK_EQUAL(str13, "1.79769e+308");

		String str14(str1), str16("foo"), str17(str1.CString());
		String str15(str1.CString(), 3);
		CHECK_EQUAL(str15, "aaa");
		CHECK_EQUAL(str16, "foo");
		CHECK_EQUAL(str14, str1);
		CHECK_EQUAL(str14, str17);
		str3 = str4;
		CHECK_EQUAL(str3, str4);
		str3 = "foobar";
		CHECK_EQUAL(str3, "foobar");
		str2 += str1;
		CHECK_EQUAL(str2, "aaaaa");
		str2 += "ba";
		CHECK_EQUAL(str2, "aaaaaba");
		str2 += 'c';
		CHECK_EQUAL(str2, "aaaaabac");
		str4 = "foo";
		str4+=1;
		CHECK_EQUAL(str4, "foo1");
		str4+=1.3;
		CHECK_EQUAL(str4, "foo11.3");
		str4+=false;
		CHECK_EQUAL(str4, "foo11.3false");

		str3 = str2 + str1;
		CHECK_EQUAL(str2, "aaaaabac");
		CHECK_EQUAL(str1, "aaaa");
		CHECK_EQUAL(str3, "aaaaabacaaaa");
		str3 = str2 + "df";
		CHECK_EQUAL(str3, "aaaaabacdf");

		str2 = "aaaa";
		CHECK(str1 == str2);
		CHECK(str1 == "aaaa");
		CHECK(str1 != "saaaa");
		CHECK(str1 != str3);
		str2 += 'w';
		CHECK(str1 < str2);
		CHECK(str1 <= str2);
		CHECK(str2 > str1);
		CHECK(str2 >= str1);
		CHECK(str1 < "asdf");
		CHECK(str1 <= "asdf");
		CHECK(str1 > "a0");
		CHECK(str1 >= "a0");
		str2 = str1;
		CHECK(str1 <= str2);
		CHECK(str1 <= "aaaa");
		CHECK(str1 >= "aaaa");
		CHECK(str2 >= str1);

		str1 = "fOobar";
		CHECK_EQUAL(str1.Replaced('o', 'a', false), "faabar");
		CHECK_EQUAL(str1, "fOobar");
		str1.Replace('o', 'a', false);
		CHECK_EQUAL(str1, "faabar");
		str1 = "fOobar";
		CHECK_EQUAL(str1.Replaced('o', 'a', true), "fOabar");
		CHECK_EQUAL(str1, "fOobar");
		str1.Replace('o', 'a', true);
		CHECK_EQUAL(str1, "fOabar");
		str1 = "fOobar";
		CHECK_EQUAL(str1.Replaced('O', 'a', true), "faobar");
		CHECK_EQUAL(str1, "fOobar");
		str1.Replace('O', 'a', true);
		CHECK_EQUAL(str1, "faobar");
		str2 = "ba";
		str3 = "";
		str1 = "fOobar";
		CHECK_EQUAL(str1.Replaced(str2, str3, true), "fOor");
		CHECK_EQUAL(str1, "fOobar");
		str1.Replace(str2, str3, true);
		CHECK_EQUAL(str1, "fOor");
		str2 = "f";
		str3 = "ad";
		str1 = "fOobar";
		CHECK_EQUAL(str1.Replaced(str2, str3, true), "adOobar");
		CHECK_EQUAL(str1, "fOobar");
		str1.Replace(str2, str3, true);
		CHECK_EQUAL(str1, "adOobar");
		str1 ="foo";
		str2 = str1;
		str1.Append("bar");
		CHECK_EQUAL(str1, "foobar");
		str1.Append(str2);
		CHECK_EQUAL(str1, "foobarfoo");
		str1.Append(str2.CString(), 2);
		CHECK_EQUAL(str1, "foobarfoofo");
		str1.Append('f');
		CHECK_EQUAL(str1, "foobarfoofof");
		str1="foobar";
		str1.Insert(0, "erm");
		CHECK_EQUAL(str1, "ermfoobar");
		str1.Insert(str1.Length(), "baz");
		CHECK_EQUAL(str1, "ermfoobarbaz");
		str2 = "wux";
		str1.Insert(3, str2);
		CHECK_EQUAL(str1, "ermwuxfoobarbaz");
		str1.Erase(3,3);
		CHECK_EQUAL(str1, "ermfoobarbaz");
		str1.Clear();
		CHECK_EQUAL(str1, "");
		str1 = "foo";
		str2 = "bar";
		str1.Swap(str2);
		CHECK_EQUAL(str1, "bar");
		CHECK_EQUAL(str2, "foo");
		str1 = "foobar";
		CHECK_EQUAL(str1.Substring(3), "bar");
		CHECK_EQUAL(str1.Substring(2,3), "oba");
		str1 = "\t foo  ";
		CHECK_EQUAL(str1.Trimmed(), "foo");
		str1 = "fOo";
		CHECK_EQUAL(str1.ToLower(), "foo");
		CHECK_EQUAL(str1.ToUpper(), "FOO");
		str1="12/01/2001";
		auto vec= str1.Split('/');
		CHECK_EQUAL(vec[0], "12");
		CHECK_EQUAL(vec[1], "01");
		CHECK_EQUAL(vec[2], "2001");
		str1.Join(vec, "-");
		CHECK_EQUAL(str1, "12-01-2001");
		
		str1 = "foobar";
		CHECK_EQUAL(str1.Find("s"), String::NPOS);
		CHECK_EQUAL(str1.Find("foo"), 0);
		CHECK_EQUAL(str1.Find("Foo"), String::NPOS);
		CHECK_EQUAL(str1.Find("Foo", 0, false), 0);
		CHECK_EQUAL(str1.Find("o"), 1);
		CHECK_EQUAL(str1.FindLast("o"), 2);
		CHECK(str1.StartsWith("foo"));
		CHECK(!str1.StartsWith("Fo"));
		CHECK(str1.StartsWith("Foo", false));
		CHECK(str1.EndsWith("bar"));
		CHECK(!str1.EndsWith("aR"));
		CHECK(str1.EndsWith("aR", false));
		str1.Clear();
		CHECK(str1.Empty());
		str1 = "fOobar";
		CHECK(str1.Contains("fOo"));
		CHECK(!str1.Contains("foo"));
		CHECK(!str1.Contains("as"));
		CHECK(str1.Contains('o'));
		
		String tstr = str1;
		String mstr1(std::move(tstr));
		CHECK_EQUAL(mstr1, str1);
		CHECK(tstr.Empty());
		tstr = str1;
		String mstr2 = std::move(tstr);
		CHECK_EQUAL(mstr2, str1);
		CHECK(tstr.Empty());

		UTF8String utf8str = "£oo", utf8str2;
		UTF8String utf8str1('£', 10);
		utf8str2 = utf8str;
		CHECK_EQUAL(utf8str,  "£oo");
		CHECK_EQUAL(utf8str1,  "££££££££££");
		CHECK_EQUAL(utf8str,  utf8str2);
		str1 = "£oo";
		CHECK_EQUAL(utf8str,  str1);
		utf8str += "£oo";
		CHECK_EQUAL(utf8str,  "£oo£oo");
		CHECK(utf8str != utf8str2);
		CHECK_EQUAL(utf8str.ToUpper(),  "£OO£OO");
		utf8str2 = utf8str.ToUpper();
		CHECK_EQUAL(utf8str2.ToLower(),  "£oo£oo");

		utf8str2.Clear();
		CHECK(utf8str2.Empty());
		CHECK_EQUAL(utf8str + utf8str,  "£oo£oo£oo£oo");
		
		utf8str = "££oo£ll££££££a";
		Vector<UTF8String> ret;
		utf8str.Split('£', ret);
		CHECK_EQUAL(ret.Size(),  3);
		CHECK_EQUAL(ret[0],  "oo");
		CHECK_EQUAL(ret[1],  "ll");
		CHECK_EQUAL(ret[2],  "a");
		
		utf8str = "foo£bar£wux";
		ret = utf8str.Split('£');
		CHECK_EQUAL(ret.Size(),  3);
		CHECK_EQUAL(ret[0],  "foo");
		CHECK_EQUAL(ret[1],  "bar");
		CHECK_EQUAL(ret[2],  "wux");
		
		utf8str = "foo£bar££££";
		ret = utf8str.Split('£');
		CHECK_EQUAL(ret.Size(),  2);
		CHECK_EQUAL(ret[0],  "foo");
		CHECK_EQUAL(ret[1],  "bar");
		
		utf8str = "£££££";
		ret = utf8str.Split('£');
		CHECK_EQUAL(ret.Size(),  0);

		WString wstring(utf8str);
		utf8str1 = wstring;
		
		CHECK_EQUAL(utf8str,  utf8str1);

		utf8str="ÃsÚs£";
		CHECK_EQUAL(utf8str.ToLower(),  "ãsús£");
		CHECK_EQUAL(utf8str.ToUpper(),  "ÃSÚS£");
		
		//test Find
		utf8str="la£§nÃsÚs£sAd£§asã";
		CHECK_EQUAL(utf8str.Find('£'),  2);
		CHECK_EQUAL(utf8str.FindLast('£'), 13);
		CHECK_EQUAL(utf8str.Find('@'), UTF8String::NPOS);
		CHECK_EQUAL(utf8str.FindLast('@'), UTF8String::NPOS);
		CHECK_EQUAL(utf8str.Find("£§"),  2);
		CHECK_EQUAL(utf8str.FindLast("£§"), 13);
		CHECK_EQUAL(utf8str.Find("ã", false), 5);
		CHECK_EQUAL(utf8str.FindLast("Ã", false), 17);

		//test Join
		utf8str = "££oo£ll££££££a";
		utf8str.Split('£', ret);
		utf8str1 ="ooúllúa";
		utf8str.Join(ret, "ú");
		CHECK_EQUAL(utf8str,  utf8str1);

		//test Replace
		utf8str = "££oo£ll££££££a";
		utf8str.Replace('£', '§');		
		CHECK_EQUAL(utf8str, "§§oo§ll§§§§§§a");
		utf8str = "££oo£ll££££££a";
		utf8str.Replace(1, '§');		
		CHECK_EQUAL(utf8str,  "£§oo£ll££££££a");
		utf8str = "ãÃfooÃâãÂ";
		utf8str.Replace('ã', 'o', false);
		CHECK_EQUAL(utf8str, "oofoooâoÂ");
		utf8str = "ãÃfooÃâãÂ";
		utf8str.Replace('ã', 'o');
		CHECK_EQUAL(utf8str, "oÃfooÃâoÂ");
		utf8str = "ÃfooúãfoowuxbarÃfoobar";
		utf8str.Replace("Ãfoo", "lstrg");
		CHECK_EQUAL(utf8str, "lstrgúãfoowuxbarlstrgbar");
		utf8str = "ÃfooúãfoowuxbarÃfoobar";
		utf8str.Replace("Ãfoo", "lstrg", false);
		CHECK_EQUAL(utf8str, "lstrgúlstrgwuxbarlstrgbar");
		utf8str = "ÃfooúãfoowuxbarÃfoobar";
		utf8str.Replace(1, 3, "replacement");
		CHECK_EQUAL(utf8str, "ÃreplacementúãfoowuxbarÃfoobar");
		utf8str.Replace(1, 11, "foo");
		CHECK_EQUAL(utf8str, "ÃfooúãfoowuxbarÃfoobar");

		//test Substring
		utf8str = "££oo£ll££££££a";
		CHECK_EQUAL(utf8str.Substring(5), "ll££££££a");
		CHECK_EQUAL(utf8str.Substring(5, 3), "ll£");


	}
}

// These typemaps cause parameters of type TYPE named ARGNAME to become IntPtr in C#.
%define %cs_marshal_intptr(TYPE, ARGNAME...)
	%typemap(ctype)  TYPE ARGNAME "void*"
	%typemap(imtype) TYPE ARGNAME "IntPtr"
	%typemap(cstype) TYPE ARGNAME "IntPtr"
	%typemap(in)     TYPE ARGNAME %{ $1 = ($1_ltype)$input; /* IntPtr */ %}
	%typemap(csin)   TYPE ARGNAME "$csinput"
	
	%typemap(out)    TYPE ARGNAME %{ $result = $1; %}
	%typemap(csout, excode=SWIGEXCODE) TYPE ARGNAME {
		IntPtr cPtr = $imcall;$excode
		return cPtr;
	}
	%typemap(csvarout, excode=SWIGEXCODE2) TYPE ARGNAME %{
		get {
			IntPtr cPtr = $imcall;$excode
			return cPtr;
		}
	%}

	%typemap(ctype)  TYPE& ARGNAME "void**"
	%typemap(imtype) TYPE& ARGNAME "ref IntPtr"
	%typemap(cstype) TYPE& ARGNAME  "ref IntPtr"
	%typemap(in)     TYPE& ARGNAME %{ $1 = ($1_ltype)$input; %}
	%typemap(csin)   TYPE& ARGNAME "ref $csinput"
%enddef

// Used to wrap type-safe enums that are namespaced inside C++ structs.
%define %cs_struct_enum(TYPE)
	%typemap(csbody) TYPE %{ %}
	%typemap(csconstruct) TYPE %{ %}
	%typemap(csdestruct, methodname="", methodmodifiers="") TYPE %{ %}
	%typemap(csfinalize) TYPE %{ %}
%enddef
/*
 Copyright (c) 2010, The Barbarian Group
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Color.h"
#include "cinder/Quaternion.h"
#include "cinder/Function.h"

#include <string>
#include <list>
#include <boost/any.hpp>

typedef struct CTwBar TwBar;

namespace cinder {

namespace app {
  class Window;
  typedef std::shared_ptr<Window>		WindowRef;
}

namespace params {
  
typedef std::shared_ptr<class InterfaceGl>	InterfaceGlRef;

class ParamBase {
  public:
	ParamBase( const std::string &name, void *voidPtr )
		: mName( name ), mVoidPtr( voidPtr ), mReadOnly( false ), mHasMin( false ), mHasMax( false ), mMin( 0 ), mMax( 0 )
	{}

	const std::string&	getName() const				{ return mName; }
	void*				getVoidPtr() const			{ return mVoidPtr; }
	bool				isReadOnly() const			{ return mReadOnly; }
	bool				hasMin() const				{ return mHasMin; }
	bool				hasMax() const				{ return mHasMax; }
	float				getMin() const				{ return mMin; }
	float				getMax() const				{ return mMax; }
	const std::string&	getKeyIncr() const			{ return mKeyIncr; }
	const std::string&	getKeyDecr() const			{ return mKeyDecr; }

protected:
	std::string mName, mKeyIncr, mKeyDecr;
	void*		mVoidPtr;
	bool		mReadOnly, mHasMin, mHasMax;
	float		mMin, mMax;
};

template <typename T>
class Param : public ParamBase {
  public:
	typedef std::function<void ( T )>	SetterFn;
	typedef std::function<T ()>			GetterFn;
	typedef std::function<void ()>		UpdateFn;

	Param( const std::string &name )
	: ParamBase( name, nullptr ), mTarget( nullptr )
	{}

	Param( const std::string &name, T *target )
		: ParamBase( name, target ), mTarget( target )
	{}

	Param&	min( float minVal )						{ mMin = minVal; mHasMin = true; return *this; }
	Param&	max( float maxVal )						{ mMax = maxVal; mHasMax = true; return *this; }
	Param&	keyIncr( const std::string &keyIncr )	{ mKeyIncr = keyIncr; return *this; }
	Param&	keyDecr( const std::string &keyDecr )	{ mKeyDecr = keyDecr; return *this; }

	Param&	setterFn( const SetterFn &setterFunction )		{ mSetterFn = setterFunction; return *this; }
	Param&	getterFn( const GetterFn &getterFunction )		{ mGetterFn = getterFunction; return *this; }
	Param&	updateFn( const UpdateFn &updateFunction )		{ mUpdateFn = updateFunction; return *this; }

	T*		getTarget() const			{ return mTarget; }
	
	const SetterFn&	getSetterFn() const	{ return mSetterFn; }
	const GetterFn&	getGetterFn() const	{ return mGetterFn; }
	const UpdateFn& getUpdateFn() const	{ return mUpdateFn; }

  private:
	T*			mTarget;
	SetterFn	mSetterFn;
	GetterFn	mGetterFn;
	UpdateFn	mUpdateFn;
};


class InterfaceGl {
  public:
	InterfaceGl() {}
	InterfaceGl( const std::string &title, const Vec2i &size, const ColorA &color = ColorA( 0.3f, 0.3f, 0.3f, 0.4f ) );
	InterfaceGl( cinder::app::WindowRef window, const std::string &title, const Vec2i &size, const ColorA &color = ColorA( 0.3f, 0.3f, 0.3f, 0.4f ) );
	
	static InterfaceGlRef create( const std::string &title, const Vec2i &size, const ColorA &color = ColorA( 0.3f, 0.3f, 0.3f, 0.4f ) );
	static InterfaceGlRef create( cinder::app::WindowRef window, const std::string &title, const Vec2i &size, const ColorA &color = ColorA( 0.3f, 0.3f, 0.3f, 0.4f ) );

	void	draw();

	void	show( bool visible = true );
	void	hide();
	bool	isVisible() const;
	
	void	maximize( bool maximized = true );
	void	minimize();
	bool	isMaximized() const;

	template <typename T>
	void	add( const Param<T> &param );
	
	void	addParam( const std::string &name, bool *boolParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, float *floatParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, double *doubleParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, int32_t *intParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, Vec3f *vectorParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, Quatf *quatParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, Color *quatParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, ColorA *quatParam, const std::string &optionsStr = "", bool readOnly = false );
	void	addParam( const std::string &name, std::string *strParam, const std::string &optionsStr = "", bool readOnly = false );
	
	void	addParam( const std::string &name, const std::function<void (bool)> &setter, const std::function<bool ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (float)> &setter, const std::function<float ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (double)> &setter, const std::function<double ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (int32_t)> &setter, const std::function<int32_t ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (Vec3f)> &setter, const std::function<Vec3f ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (Quatf)> &setter, const std::function<Quatf ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (Color)> &setter, const std::function<Color ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (ColorA)> &setter, const std::function<ColorA ()> &getter, const std::string &optionsStr = "" );
	void	addParam( const std::string &name, const std::function<void (std::string)> &setter, const std::function<std::string ()> &getter, const std::string &optionsStr = "" );
	
	//! Adds enumerated parameter. The value corresponds to the indices of \a enumNames.
	void	addParam( const std::string &name, const std::vector<std::string> &enumNames, int *param, const std::string &optionsStr = "", bool readOnly = false );
	
	void	addSeparator( const std::string &name = "", const std::string &optionsStr = "" );
	void	addText( const std::string &name = "", const std::string &optionsStr = "" );
	void	addButton( const std::string &name, const std::function<void()> &callback, const std::string &optionsStr = "" );
	void	removeParam( const std::string &name );
	//! Removes all the variables, buttons and separators previously added.
	void	clear();
	void	setOptions( const std::string &name = "", const std::string &optionsStr = "" );

  protected:
	void	init( app::WindowRef window, const std::string &title, const Vec2i &size, const ColorA color );
	void	implAddParam( const std::string &name, void *param, int type, const std::string &optionsStr, bool readOnly );

	template <typename T>
	void	implAddParam( const Param<T> &param, int type );


	template <class T>
	void implAddParamCb( const std::string &name, int type, const std::string &optionsStr, const std::function<void (T)> &setter, const std::function<T ()> &getter );

	std::weak_ptr<app::Window>		mWindow;
	std::shared_ptr<TwBar>			mBar;
	int								mTwWindowId;
	
	std::list<boost::any>			mStoredCallbacks;
};

} } // namespace cinder::params

#pragma once

#include <assert.h>

//////////////////////////////////////////////////////////////////////////

class Param
{
public:
	Param() { m_typeCode = 0; }
	virtual ~Param() {}

	int m_typeCode;

	virtual bool isValid() { return false; }
};

class ArParam : public Param
{
public:
	ARTP_HEADER* m_pInfo;
	ArParam( ARTP_HEADER* pInfo ) : m_pInfo(pInfo)
	{		
	}

private:
	ArParam() :	m_pInfo(0)
	{		
	}
};

class SimpleParam : public Param
{
public:
	LONG	m_code;
	INT_PTR	m_result;
	INT		m_detailCode;

	SimpleParam( LONG code_, INT_PTR ret_, INT detCode_ ) : 
	m_code(code_), m_result(ret_), m_detailCode(detCode_) {
		m_typeCode = 1;
	}

private:
	SimpleParam() :
	   m_code(-1), m_result(-1), m_detailCode(-1) {
		   m_typeCode = 1;
	   }
};


//////////////////////////////////////////////////////////////////////////

/**
 * This struct includes functor information.
 * @see IFunctor::Info()
 */
struct FINFO
{
	void*	pObj;	/**< An object's pointer	*/
	void*	pFunc;	/**< An function's pointer	*/
};

/**
 * This interface represents a functor.
 */
struct IFunctor
{
	virtual int		Call( Param& param_ ) = 0;
	virtual void	Release() = 0;
	virtual FINFO	Info() = 0;
};


/** A function pointer type for IFunctor  */
typedef int (*P_SFUNC)(Param&);

/** A member function pointer type for IFunctor */
template<typename T>
class TYPE
{
public:
	typedef int (T::*P_MFUNC)(Param&);
};

/** A utility function for converting a member-function's pointer */
template <typename Dest, typename Src>
Dest power_cast(Src pFunc)
{
	union Convert
	{
		Dest	target;
		Src		src;
	} convert;
	
	convert.src = pFunc;
	return convert.target;
}


// For member functions
template<typename T>
class CMFunctor : public IFunctor
{
public:
	CMFunctor(T* pObj, typename TYPE<T>::P_MFUNC pFunc)
	{
		_ASSERTE( pObj && pFunc);

		m_pObj	= pObj;
		m_pFunc	= pFunc;
	}

	virtual int Call(Param& param)
	{
		return (m_pObj->*m_pFunc)(param);
	}

	virtual void Release()
	{
		// This makes the class use the new and delete operator of the same CRT.
		delete this;
	}

	virtual FINFO	Info()
	{
		FINFO finfo;
		finfo.pFunc = power_cast<void*>( m_pFunc );
		finfo.pObj	= reinterpret_cast<void*>( m_pObj );
		return finfo;
	}

protected:
	T*							m_pObj;
	typename TYPE<T>::P_MFUNC	m_pFunc;

private:
	/* create instance only on heap memory*/
	~CMFunctor()
	{
	}
};

// For member functions
template<typename T>
inline IFunctor* Fnt(T* pObj, typename TYPE<T>::P_MFUNC	pFunc)
{
//#ifdef _DEBUG	
//	return DEBUG_NEW CMFunctor<T>(pObj, pFunc);
//#else
	return new CMFunctor<T>(pObj, pFunc);
//#endif
}

/**
 * Utility class for IFunctor.
 */
class CFunctor
{
public:
	CFunctor()
	{
		m_pFnt = NULL; 
	}
	~CFunctor()
	{
		if ( m_pFnt )
			m_pFnt->Release();
	}

	void operator=(IFunctor* fnt)
	{ 
		if( m_pFnt ) m_pFnt->Release();
		m_pFnt = fnt;
	}

	int Call( Param& parm )
	{
		if ( m_pFnt ) { 
			return m_pFnt->Call( parm ); 
		} 
		return 0; 
	}
	IFunctor* Get() const	{ return m_pFnt; }

protected:
	IFunctor*	m_pFnt;
};

// For non-member functions
class CSFunctor : public IFunctor
{
public:
	CSFunctor(P_SFUNC pFunc)
	{
		assert( pFunc);
		m_pFunc = pFunc;
	}

	virtual int Call(Param& param)
	{
		return (*m_pFunc)(param);
	}

	virtual void Release()
	{
		// This makes the class use the new and delete operator of the same CRT.
		delete this;
	}

	virtual FINFO	Info()
	{
		FINFO finfo;
		finfo.pFunc = reinterpret_cast<void*>( m_pFunc);
		finfo.pObj	= NULL;
		return finfo;
	}

protected:
	P_SFUNC	m_pFunc;

private:
	/* create instance only on heap memory*/
	~CSFunctor()
	{
	}
};

// For non-member functions
inline IFunctor* Fnt(P_SFUNC pFunc)
{
	return new CSFunctor(pFunc);
}

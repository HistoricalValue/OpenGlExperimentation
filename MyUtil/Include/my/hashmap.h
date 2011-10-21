#ifndef __MY__HASH_MAP__H__
#define __MY__HASH_MAP__H__

#pragma warning( push, 0 )
#	include "DDebug.h"
#	include <algorithm>
#	include <MyUtils.h>
#pragma warning( pop )

///////////////////////////////////////////////////////////

namespace my	{

///////////////////////////////////////////////////////////

template <typename K>
struct hashmap_key_traits {
	typedef K				key_type;

	static unsigned int	hash (key_type const k)
							{ return k; }
};

template <typename T>
struct hashmap_traits {
	typedef T						value_type;
	typedef value_type const		const_value_type;
	typedef	value_type*				pointer;
	typedef const_value_type*		const_pointer;
	typedef value_type&				reference;
	typedef const_value_type&		const_reference;

	typedef Comparator				std::equal<T>;
};

template <typename K, typename T, typename ComparatorType = typename hashmap_traits<T>::Comparator>
class hashmap {
public:
	typedef hashmap_traits<T>		traits;
	typedef hashmap_key_traits<K>	key_traits;
	typedef ComparatorType			Comparator;

	T&	operator [] (K const& key);

	
private:
	struct node {
		node*			prev, next;
		K				key;
		dptr<T> const	ptr;

		void			AppendTo (node*);
		void			PrependTo (node*);
		node*			Find (K const&, Comparator const&);

		node (K const&, T*);
	};

	node* const		lists[997];
	// non-state
	Comparator		cmp;
};

template <typename K, typename T, typename C>
void hashmap<K, T, C>::node::AppendTo (node* const _other) {
	DASSERT(DPTR(other)->next == NULL && prev == NULL && next == NULL);
	DPTR(other)->next = DPTR(this);
	prev = DPTR(other);
}

template <typename K, typename T, typename C>
void hashmap<K, T, C>::node::PrependTo (node* const other) {
	DASSERT(DPTR(other)->prev == NULL && prev == NULL && next == NULL);
	DPTR(other)->prev = DPTR(this);
	next = DPTR(other);
}

template <typename K, typename T, typename C>
hashmap<K, T, C>::node* hashmap<K, T, C>::node::Find (K const& key, Comparator const& cmp) {
	node* result;

	if (cmp(node::key, key))
		result = this
	else
		if (!next.isnull())
			result = next->Find(key, cmp);
		else
			result = NULL;

	return result;
}

template <typename K, typename T, typename ComparatorType>
T& hashmap::operator [] (K const& key) {
	size_t const	h(key_traits::hash(k)),
					i(h % countof(lists));
	node*			found(NULL);

	if (lists[i] == NULL) {
		found = DNEWCLASS(node, (key, DNEW(T)));
		lists[i] = found;
	}
	else {
		found = lists[i].Find(key, cmp);
		if (found == NULL) {
			found = DNEWCLASS(node, (key, DNEW(T)))
			found->PrependTo(lists[i]);
		}
	}

	DASSERT(lists[i].Find(key, cmp) == found);

	return *DPTR(found)->ptr.native();
}

///////////////////////////////////////////////////////////

}	// my

///////////////////////////////////////////////////////////

#endif // __MY__HASH_MAP__H__

#ifndef __MY__LIST__H__
#define __MY__LIST__H__

#include <MyUtil.h>
#include <PPlaceholder.h>
#include <PAssert.h>
#include <PProtectedClass.h>
#include <PTypes.h>

#define MY_LIST_CAPACITY 10

namespace my {

#pragma warning( push )
#pragma warning( disable: 4351 ) // elements of "elements" will be default initialised
#pragma warning( disable: 4251 ) // allocator(DefaultAllocator<T>) needs dll-interface

    //
    // Allocator used only to construct/destruct objects.
    //
    template <typename T, typename Alloc>
    class MYUTIL_API ProtectiveList {
    public:
        typedef Alloc                                    Allocator;
        typedef    T                                        Value;
        typedef Value*                                    Pointer;
        typedef Value&                                    Reference;
        typedef T const                                    ConstValue;
        typedef ConstValue*                                ConstPointer;
        typedef ConstValue&                                ConstReference;

        class const_iterator;
        friend class const_iterator;
    private:
        typedef P_PROTECTED_CLASS_WITH_ALLOC(Value, Allocator)
                                                        Element;
        inline Element*                                    Elements (void) { return reinterpret_cast<Element*>(&elements[0]); }
        inline Element const*                            Elements (void) const { return reinterpret_cast<Element const*>(&elements[0]); }
    public:

        class const_iterator {
        public:
                                                        const_iterator (Element const* const _i, Element const* const _end):
                                                            i(_i), end(_end) { PASSERT( i <= end) }
            explicit                                    const_iterator (Element const* const _end):
                                                            i(_end), end(_end) { }
                                                        const_iterator (const_iterator const& o):
                                                            i(o.i), end(o.end) { PASSERT( i <= end ) }

            bool                                        IsAtTheEnd (void) const { return i == end;  }
            ConstPointer                                operator -> (void) const { PASSERT(!IsAtTheEnd()) return &P_PROTECTED_CLASS_CONSTVAL(*i); }
            ConstReference                                operator * (void) const { return *this->operator ->(); }
            const_iterator&                                operator ++ (void) { PASSERT(!IsAtTheEnd()) ++i; return *this; }

            bool                                        operator == (const_iterator const& other) { return i == other.i; }
            bool                                        operator != (const_iterator const& other) { return !this->operator ==(other); }
        private:
            Element const*                                i;
            Element const* const                        end;

            void operator = (const_iterator const&);
        };

        const_iterator                                    begin (void) const { return const_iterator(&Elements()[1], &Elements()[i+1]); }
        const_iterator                                    end (void) const { return const_iterator(&Elements()[i+1]); }

        void                                            push_back (ConstReference el) {
                                                            PASSERT(!IsFull())
                                                            new(&Elements()[++i]) Element(el);
                                                        }

        bool                                            IsFull (void) const { return i == MY_LIST_CAPACITY + 2-1 - 1; }


                                                        ProtectiveList (void):
                                                            elements(),
                                                            i(0),
                                                            allocator(),
                                                            valueview(reinterpret_cast<Element const (&)[MY_LIST_CAPACITY + 2]>(elements)) {
                                                            memset(&elements[0], 0x00, sizeof(elements));
                                                        }

                                                        ProtectiveList (Allocator const& _allocator):
                                                            elements(),
                                                            i(0),
                                                            allocator(_allocator),
                                                            valueview(reinterpret_cast<Element const (&)[MY_LIST_CAPACITY + 2]>(elements)) {
                                                            memset(&elements[0], 0x00, sizeof(elements));
                                                        }

                                                        ProtectiveList (ProtectiveList<Value,Allocator> const& other):
                                                            elements(),
                                                            i(other.i),
                                                            allocator(other.allocator),
                                                            valueview(reinterpret_cast<Element const (&)[MY_LIST_CAPACITY + 2]>(elements)) {
                                                            memcpy(&elements[0], &other.elements[0], sizeof(elements));
                                                        }

                                                        ~ProtectiveList (void) {
#ifdef P_DEBUG
                                                            char __zeros[sizeof(Element)]; // TODO put somewhere constant
                                                            memset(&__zeros[0], 0x00, sizeof(__zeros));

                                                            Element const* elements(Elements());
                                                            for (unsigned int j(i+1); j < MY_LIST_CAPACITY + 2; ++j)
                                                                PASSERT(memcmp(&__zeros[0], &elements[j], sizeof(Element)) == 0)

                                                            for (unsigned int j(1); j < i+1; ++j)
                                                                PASSERT(elements[j].IsValid())
#endif
                                                        }
    private:
        codeshare::utilities::Byte                        elements[sizeof(Element) * (MY_LIST_CAPACITY + 2)];
        unsigned int                                    i;
        Allocator                                        allocator;
        Element const                                    (&valueview)[MY_LIST_CAPACITY + 2];

        void operator = (ProtectiveList<T,Alloc> const&);
    };

#pragma warning( pop )

}

#endif // __MY__LIST__H__

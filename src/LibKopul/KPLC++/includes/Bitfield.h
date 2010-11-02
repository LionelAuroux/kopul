#ifndef __BITFIELD___H
# define __BITFIELD___H
# include "StaticType.h"

namespace kpl
{
	class	Bitfield : public StaticType
	{
	public:
            Bitfield(int size = 32);
            Bitfield(const Bitfield&);
            ~Bitfield();
            Bitfield&           operator = (const Bitfield&);
            int                 GetSize() const;
            int                 GetSizeInOctet() const;
            void                SetSize(int size);

            // Get a string representation of the object
            virtual const std::string&  ToString() const;

            // Get the type of the object
            virtual const std::string&  GetType() const;

            // returns true if the given type and content are equal.
            virtual bool                Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
            virtual IObject*            Clone() const;

        protected:

        private:
            int                 _size;
            std::string         _objectToStr;
            std::string         _objectType;
	};
};

#endif //__BITFIELD___H

/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */


template<typename HandleType>
void Handled<HandleType>::SetHandle(HandleType handle)
{
	this->handle = handle;
}

template<typename HandleType>
const HandleType& Handled<HandleType>::GetHandle() const
{
	return handle;
}
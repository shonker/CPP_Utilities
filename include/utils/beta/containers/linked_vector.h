#pragma once
#include <memory>
#include <utility>
#include <type_traits>
#include <vector>

#include "../../memory.h"
#include "segment.h"

namespace utils::beta::containers
	{
	namespace details
		{
		template<typename T, size_t inner_size, typename Allocator>
		struct linked_vector_iterator;
		}

	template <typename T, size_t inner_size = 8, typename Allocator = std::allocator<T>>
	class linked_vector
		{
		friend struct details::linked_vector_iterator        <      T, inner_size, Allocator>;
		friend struct details::linked_vector_iterator        <const T, inner_size, Allocator>;
		friend struct details::linked_vector_reverse_iterator<      T, inner_size, Allocator>;
		friend struct details::linked_vector_reverse_iterator<const T, inner_size, Allocator>;

		private:
			using segment_t = segment<T, inner_size>;
			using segment_ptr_t = utils::observer_ptr<segment_t>;
			using segment_allocator_t = typename std::allocator_traits<Allocator>::template rebind_alloc<segment_t>;

		public:
			using value_type      = segment_t::value_type;
			using reference       = segment_t::reference;
			using const_reference = segment_t::const_reference;
			using pointer         = segment_t::pointer;
			using const_pointer   = segment_t::const_pointer;

			using iterator               = details::linked_vector_iterator        <      T, inner_size, Allocator>;
			using const_iterator         = details::linked_vector_iterator        <const T, inner_size, Allocator>;
			using reverse_iterator       = details::linked_vector_reverse_iterator<      T, inner_size, Allocator>;
			using const_reverse_iterator = details::linked_vector_reverse_iterator<const T, inner_size, Allocator>;

			linked_vector() {}
			linked_vector(const Allocator& allocator) : segment_allocator{allocator} {}

			~linked_vector() { clear(); }

			inline size_t capacity() const noexcept { return _segments_count * inner_size; }
			inline size_t size()     const noexcept
				{
				if (segment_ptr_t segment = last_segment){ return (segments_count() - 1) * inner_size + segment->size; }
				return 0;
				}
			inline size_t segments_count() const { return _segments_count; }

			inline T& front() { return first_segment->arr[0]; }
			inline T& back()  { return last_segment ->arr[last_segment->size - 1];  }

			inline bool empty() const { return first_segment == nullptr;  }
			inline void clear()
				{
				if (segment_ptr_t segment_ptr = first_segment)
					{
					while (segment_ptr != last_segment)
						{
						segment_ptr_t next_segment = segment_ptr->next;
						segment_ptr->~segment_t(); //call destructor so individual elements within the array are destroyed automatically
						std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segment_ptr, 1);
						segment_ptr = next_segment;
						}
					// last segment doesn't call the destructor so individual elements within the array are NOT destroyed automatically
					for (auto it{ segment_ptr->begin() }; it != (segment_ptr->begin() + segment_ptr->size); it++)
						{
						it->~T();
						}
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, segment_ptr, 1);
					first_segment = nullptr;
					last_segment  = nullptr;
					_segments_count = 0;
					}
				}

			inline T& push(const value_type& value)
				{
				auto new_t{ new(get_free_slot()) T(value) };
				last_segment->size++;
				return *new_t;
				}

			template <typename ...Args>
			inline T& emplace(Args&&... args) 
				{
				auto new_t{ new(get_free_slot()) T(std::forward<Args>(args)...) };
				last_segment->size++;
				return *new_t;
				}

			iterator erase(const_iterator erase_from, const_iterator erase_to)
				{
				iterator erasing_it{ erase_from };
				iterator move_from { erase_to + 1 };
				
				while (move_from != end())
					{
					*erasing_it = std::move(*move_from);
					erasing_it++;
					move_from++;
					}
				
				iterator new_end{ erasing_it };
				
				while (erasing_it != end())
					{
					erasing_it->~T();
					erasing_it++;
					}
				
				//update size
				auto new_last_segment_ptr{ new_end.segment_ptr };
				
				while (new_last_segment_ptr != last_segment)
					{
					auto delete_segment_ptr{ last_segment };
					last_segment = last_segment->prev;
					std::allocator_traits<segment_allocator_t>::deallocate(segment_allocator, delete_segment_ptr, 1);
					}
				new_last_segment_ptr->size = new_end.segment_iterator - new_last_segment_ptr->begin();
				new_last_segment_ptr->next = nullptr;
				
				return { erase_from };
				}

			inline void move_storage(T* dest, T* from, size_t n)
				{
				// Implying we are doing this within the capacity/size of the container
				if (dest < from)
					{
					T* _dest = dest, * _from = from;
					for (size_t i = 0; i < n; i++)
						*_dest++ = std::move(*_from++);
					}
				else if (dest > from)
					{
					T* _dest = dest + n - 1, * _from = from + n - 1;
					for (size_t i = n; i > 0; i--)
						*_dest-- = std::move(*_from--);
					}
				else
					return;
				}
				
			const_iterator         cbegin () const { if (first_segment) { return { first_segment->cbegin ()                                    , first_segment }; } else { return {}; } }
			const_iterator         begin  () const { if (first_segment) { return { first_segment->cbegin ()                                    , first_segment }; } else { return {}; } }
			iterator               begin  ()       { if (first_segment) { return { first_segment->begin  ()                                    , first_segment }; } else { return {}; } }


 			const_iterator         cend   () const { if (first_segment) { return { last_segment ->cbegin () +  last_segment->size              , last_segment  }; } else { return {}; } }
			const_iterator         end    () const { if (first_segment) { return { last_segment ->cbegin () +  last_segment->size              , last_segment  }; } else { return {}; } }
			iterator               end    ()       { if (first_segment) { return { last_segment ->begin  () +  last_segment->size              , last_segment  }; } else { return {}; } }

			const_reverse_iterator crbegin() const { if (first_segment) { return { last_segment ->crbegin() + (inner_size - last_segment->size), last_segment  }; } else { return {}; } }
			const_reverse_iterator rbegin () const { if (first_segment) { return { last_segment ->crbegin() + (inner_size - last_segment->size), last_segment  }; } else { return {}; } }
			reverse_iterator       rbegin ()       { if (first_segment) { return { last_segment ->rbegin () + (inner_size - last_segment->size), last_segment  }; } else { return {}; } }

			const_reverse_iterator crend  () const { if (first_segment) { return { first_segment->crend  ()                                    , first_segment }; } else { return {}; } }
			const_reverse_iterator rend   () const { if (first_segment) { return { first_segment->crend  ()                                    , first_segment }; } else { return {}; } }
			reverse_iterator       rend   ()       { if (first_segment) { return { first_segment->rend   ()                                    , first_segment }; } else { return {}; } }

		protected:
			segment_ptr_t  first_segment {nullptr};
			segment_ptr_t  last_segment  {nullptr};

			size_t              _segments_count{0};
			segment_allocator_t segment_allocator ;

			inline const segment_ptr_t get_first() const { return first_segment; }
			inline       segment_ptr_t get_first()       { return first_segment; }

			inline const segment_ptr_t get_last () const { return last_segment ; }
			inline       segment_ptr_t get_last ()       { return last_segment ; }

			pointer get_free_slot()
				{
				segment_ptr_t segment_ptr{ grow_if_full() };
				segment_t& segment{ *segment_ptr };
				return segment.arr.data() + segment.size;
				}

			segment_ptr_t grow_if_full()
				{
				if (!first_segment) { return grow_first(); }
				else if (last_segment->size == inner_size) { return grow(); }
				else { return last_segment; }
				}

			segment_ptr_t grow_first()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				new_segment->size = 0;
				new_segment->prev = nullptr;
				new_segment->next = nullptr;

				first_segment = new_segment;
				last_segment = new_segment;

				return new_segment;
				}

			segment_ptr_t grow()
				{
				segment_ptr_t new_segment{ std::allocator_traits<segment_allocator_t>::allocate(segment_allocator, 1) };
				last_segment->next = new_segment;
				new_segment->size = 0;
				new_segment->prev = last_segment;
				new_segment->next = nullptr;
				last_segment = new_segment;

				return new_segment;
				}
			
			static size_t distance(segment_ptr_t leftmost, segment_ptr_t rightmost)
				{
				segment_ptr_t curr_segment_ptr{ rightmost };
				size_t segment_distance{ 0 };

				while (curr_segment_ptr != leftmost)
					{
					curr_segment_ptr = curr_segment_ptr->prev;
					segment_distance++;
					}

				return segment_distance;
				}
		};

		
		namespace details
			{
			template<typename T, size_t inner_size, typename Allocator>
			struct linked_vector_iterator
				{
				template <typename T, size_t inner_size, typename Allocator>
				friend class linked_vector;
				template <typename T, size_t inner_size, typename Allocator>
				friend struct linked_vector_iterator;
				
				using segment_iterator_t = segment_iterator<T, inner_size>;
				using segment_ptr_t      = linked_vector   <std::remove_cv_t<T>, inner_size, Allocator>::segment_ptr_t;

				public:
					using self_type         = linked_vector_iterator<T, inner_size, Allocator>;
					using value_type        = T  ;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					linked_vector_iterator() : segment_iterator{ nullptr }, segment_ptr{ nullptr } { }
					linked_vector_iterator(segment_iterator_t segment_iterator, const segment_ptr_t segment_ptr) : segment_iterator{ segment_iterator }, segment_ptr{ segment_ptr } { }

					template<typename rhs_T>
						requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
					linked_vector_iterator(const linked_vector_iterator<rhs_T, inner_size, Allocator>& other) : segment_iterator{ other.segment_iterator }, segment_ptr{ other.segment_ptr } {}

					template<typename rhs_T>
						requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
					self_type& operator=(const linked_vector_iterator<rhs_T, inner_size, Allocator>& other)
						{
						segment_iterator = other.segment_iterator;
						segment_ptr      = other.segment_ptr;
						return *this;
						}

					self_type  operator+ (difference_type rhs)
						{
						auto ret{ *this };


						auto to_end{ segment_ptr->end() - segment_iterator };
						if (rhs < to_end)
							{
							ret.segment_iterator += rhs;
							return ret;
							}

						auto leftover{ rhs - to_end };

						while(true)
							{
							if (!ret.segment_ptr->next) { return { ret.segment_ptr->end(), ret.segment_ptr }; }

							ret.segment_ptr = ret.segment_ptr->next;
							if (leftover < inner_size) { break; }
							
							leftover -= inner_size;
							}

						ret.segment_iterator = ret.segment_ptr->begin() + leftover;

						return ret;
						}
					self_type  operator- (difference_type rhs)
						{
						auto ret{ *this };

						auto to_begin{ segment_iterator - segment_ptr->begin() };
						if (rhs <= to_begin)
							{
							ret.segment_iterator -= rhs;
							return ret;
							}

						auto leftover{ rhs - to_begin };
						ret.segment_ptr = ret.segment_ptr->prev;

						while (leftover > inner_size)
							{
							ret.segment_ptr = ret.segment_ptr->prev;
							leftover -= inner_size;
							}

						ret.segment_iterator = ret.segment_ptr->end() - leftover;

						return ret;
						}

					template<typename rhs_T>
						requires std::same_as <std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
					difference_type operator- (const linked_vector_iterator<rhs_T, inner_size, Allocator>& rhs) const noexcept 
						{ 
						if (segment_ptr == rhs.segment_ptr)
							{
							return segment_iterator - rhs.segment_iterator;
							}

						auto a{ this->segment_ptr };
						auto b{ rhs  .segment_ptr };

						size_t iterations{ 1 };
						while(a->prev)
							{
							a = a->prev;

							if (a == rhs.segment_ptr)
								{
								difference_type a_to_begin{ this->segment_iterator   - this->segment_ptr->begin() };
								difference_type b_to_end  { rhs  .segment_ptr->end() - rhs  .segment_iterator     };
								return  static_cast<difference_type>(a_to_begin + iterations + b_to_end);
								}
							iterations++;
							}
							
						iterations = 1;
						while (b->prev)
							{
							b = b->prev;

							if (b == this->segment_ptr)
								{
								difference_type a_to_end  { this->segment_ptr->end() - this->segment_iterator};
								difference_type b_to_begin{ rhs  .segment_iterator   - rhs.segment_ptr->begin()  };
								return -static_cast<difference_type>(a_to_end + iterations + b_to_begin);
								}
							iterations++;
							}
						return {}; // this should not happen!
						}

					self_type& operator+=(difference_type rhs) { *this = *this + rhs; return *this; }
					self_type& operator-=(difference_type rhs) { *this = *this - rhs; return *this; }

					self_type operator++()
						{
						auto ret{ *this };
						operator+=(1);
						return ret;
						}

					self_type& operator++(int) { operator++(); return *this; }

					self_type  operator--()
						{
						auto ret{ *this };
						operator-=(1);
						return ret;
						}

					self_type& operator--(int) { operator--(); return *this; }
					
					const_reference operator* () const noexcept                             { return *segment_iterator; }
					reference       operator* ()       noexcept requires(!std::is_const<T>) { return *segment_iterator; }
					const_pointer   operator->() const noexcept                             { return  segment_iterator.operator->(); }
					pointer         operator->()       noexcept requires(!std::is_const<T>) { return  segment_iterator.operator->(); }
					bool operator==(const self_type& rhs) const noexcept { return segment_iterator == rhs.segment_iterator; }
					bool operator!=(const self_type& rhs) const noexcept { return segment_iterator != rhs.segment_iterator; }
					bool operator< (const self_type& rhs) const noexcept 
						{ 
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator < rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->prev;
							rhs_backward  = rhs_backward ->prev;
							if (!this_backward) { return true  ;}
							if (!rhs_backward ) { return false ;}
							}
						}
					bool operator> (const self_type& rhs) const noexcept
						{
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator > rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->prev;
							rhs_backward  = rhs_backward ->prev;
							if (!this_backward) { return false ;}
							if (!rhs_backward ) { return true  ;}
							}
						}
				private:
					segment_iterator_t segment_iterator;
					segment_ptr_t      segment_ptr;
				};

			template<typename T, size_t inner_size, typename Allocator>
			struct linked_vector_reverse_iterator
				{
				template <typename T, size_t inner_size, typename Allocator>
				friend class linked_vector;
				template <typename T, size_t inner_size, typename Allocator>
				friend struct linked_vector_reverse_iterator;
				
				using segment_iterator_t = segment_reverse_iterator<T, inner_size>;
				using segment_ptr_t      = linked_vector   <std::remove_cv_t<T>, inner_size, Allocator>::segment_ptr_t;

				public:
					using self_type         = linked_vector_reverse_iterator<T, inner_size, Allocator>;
					using value_type        = T  ;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					linked_vector_reverse_iterator() : segment_iterator{ nullptr }, segment_ptr{ nullptr } { }
					linked_vector_reverse_iterator(segment_iterator_t segment_iterator, const segment_ptr_t segment_ptr) : segment_iterator{ segment_iterator }, segment_ptr{ segment_ptr } { }

					template<typename rhs_T>
						requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
					linked_vector_reverse_iterator(const linked_vector_reverse_iterator<rhs_T, inner_size, Allocator>& other) : segment_iterator{ other.segment_iterator }, segment_ptr{ other.segment_ptr } {}

					template<typename rhs_T>
						requires std::same_as < std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
					self_type& operator=(const linked_vector_reverse_iterator<rhs_T, inner_size, Allocator>& other)
						{
						segment_iterator = other.segment_iterator;
						segment_ptr      = other.segment_ptr;
						return *this;
						}

					self_type  operator+ (difference_type rhs)
						{
						auto ret{ *this };

						auto to_end{ segment_ptr->rend() - segment_iterator };
						if (rhs < to_end)
							{
							ret.segment_iterator += rhs;
							return ret;
							}

						auto leftover{ rhs - to_end };

						while (true)
							{
							if (!ret.segment_ptr->prev) { return { ret.segment_ptr->rend(), ret.segment_ptr }; }

							ret.segment_ptr = ret.segment_ptr->prev;
							if (leftover < inner_size) { break; }

							leftover -= inner_size;
							}

						ret.segment_iterator = ret.segment_ptr->rbegin() + leftover;

						return ret;
						}
					self_type  operator- (difference_type rhs)
						{
						auto ret{ *this };

						auto to_begin{ segment_iterator - segment_ptr->rbegin() };
						if (rhs <= to_begin)
							{
							ret.segment_iterator -= rhs;
							return ret;
							}

						auto leftover{ rhs - to_begin };
						ret.segment_ptr = ret.segment_ptr->next;

						while (leftover > inner_size)
							{
							ret.segment_ptr = ret.segment_ptr->next;
							leftover -= inner_size;
							}

						ret.segment_iterator = ret.segment_ptr->rend() - leftover;

						return ret;
						}

					template<typename rhs_T>
						requires std::same_as <std::remove_cv_t<T>, std::remove_cv_t<rhs_T>>
					difference_type operator- (const linked_vector_reverse_iterator<rhs_T, inner_size, Allocator>& rhs) const noexcept 
						{ 
						if (segment_ptr == rhs.segment_ptr)
							{
							return segment_iterator - rhs.segment_iterator;
							}

						auto a{ this->segment_ptr };
						auto b{ rhs  .segment_ptr };

						size_t iterations{ 1 };
						while(a->next)
							{
							a = a->next;

							if (a == rhs.segment_ptr)
								{
								difference_type a_to_begin{ this->segment_iterator   - this->segment_ptr->rbegin() };
								difference_type b_to_end  { rhs  .segment_ptr->rend() - rhs  .segment_iterator     };
								return  static_cast<difference_type>(a_to_begin + iterations + b_to_end);
								}
							iterations++;
							}
							
						iterations = 1;
						while (b->next)
							{
							b = b->next;

							if (b == this->segment_ptr)
								{
								difference_type a_to_end  { this->segment_ptr->rend() - this->segment_iterator};
								difference_type b_to_begin{ rhs  .segment_iterator   - rhs.segment_ptr->rbegin()  };
								return -static_cast<difference_type>(a_to_end + iterations + b_to_begin);
								}
							iterations++;
							}
						return {}; // this should not happen!
						}

					self_type& operator+=(difference_type rhs) { *this = *this + rhs; return *this; }
					self_type& operator-=(difference_type rhs) { *this = *this - rhs; return *this; }

					self_type operator++()
						{
						auto ret{ *this };
						operator+=(1);
						return ret;
						}

					self_type& operator++(int) { operator++(); return *this; }

					self_type  operator--()
						{
						auto ret{ *this };
						operator-=(1);
						return ret;
						}

					self_type& operator--(int) { operator--(); return *this; }
					
					const_reference operator* () const noexcept                             { return *segment_iterator; }
					reference       operator* ()       noexcept requires(!std::is_const<T>) { return *segment_iterator; }
					const_pointer   operator->() const noexcept                             { return  segment_iterator.operator->(); }
					pointer         operator->()       noexcept requires(!std::is_const<T>) { return  segment_iterator.operator->(); }
					bool operator==(const self_type& rhs) const noexcept { return segment_iterator == rhs.segment_iterator; }
					bool operator!=(const self_type& rhs) const noexcept { return segment_iterator != rhs.segment_iterator; }
					bool operator< (const self_type& rhs) const noexcept 
						{ 
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator < rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->next;
							rhs_backward  = rhs_backward ->next;
							if (!this_backward) { return true  ;}
							if (!rhs_backward ) { return false ;}
							}
						}
					bool operator> (const self_type& rhs) const noexcept
						{
						if (segment_ptr == rhs.segment_ptr) { return segment_iterator > rhs.segment_iterator; }

						segment_ptr_t this_backward{ segment_ptr }, rhs_backward{ rhs.segment_ptr };
						while (true)
							{
							this_backward = this_backward->next;
							rhs_backward  = rhs_backward ->next;
							if (!this_backward) { return false ;}
							if (!rhs_backward ) { return true  ;}
							}
						}
				private:
					segment_iterator_t segment_iterator;
					segment_ptr_t      segment_ptr;
				};
			}
	}
#pragma once

#include <vector>
#include <stdexcept>

namespace utils
	{
	enum matrix_memory { width_first, height_first };

	template <typename T, matrix_memory MEMORY = matrix_memory::width_first, class Allocator = std::allocator<T>>
	class matrix_dyn
		{
		using inner_container_t = std::vector<T, Allocator>;

		public:
			using value_type             = inner_container_t::value_type;
			using size_type              = inner_container_t::size_type;
			using reference              = inner_container_t::reference;
			using const_reference        = inner_container_t::const_reference;
			using pointer                = inner_container_t::pointer;
			using const_pointer          = inner_container_t::const_pointer;
			using iterator               = inner_container_t::iterator;
			using const_iterator         = inner_container_t::const_iterator;
			using reverse_iterator       = inner_container_t::reverse_iterator;
			using const_reverse_iterator = inner_container_t::const_reverse_iterator;

			struct coords_t { size_type x; size_type y; };

			matrix_dyn(size_type width, size_type height) : _width(width), _height(height), _arr(width* height) {}

			const size_type width()  const noexcept { return _width; }
			const size_type height() const noexcept { return _height; }
			const size_type size()   const noexcept { return _width * _height; }

			const_pointer data() const noexcept { return _arr.data(); }
			      pointer data()       noexcept { return _arr.data(); }

			const inner_container_t& get_vector() const noexcept { return _arr; }
			      inner_container_t& get_vector()       noexcept { return _arr; }

			size_type get_index(size_type x, size_type y) const noexcept
				{
				if constexpr (MEMORY == matrix_memory::width_first) { return x + (y * _width); }
				else { return y + (x * _height); }
				}
			size_type get_x     (size_type index) const noexcept { if constexpr (MEMORY == matrix_memory::width_first) { return index % width(); } else { return index / height(); } } //TODO test
			size_type get_y     (size_type index) const noexcept { if constexpr (MEMORY == matrix_memory::width_first) { return index / width(); } else { return index % height(); } } //TODO test
			coords_t  get_coords(size_type index) const noexcept { return {get_x(index), get_y(index)}; }
			
			const_reference operator[](size_type i)      const noexcept { return _arr[i]; }
			      reference operator[](size_type i)            noexcept { return _arr[i]; }
			const_reference operator[](coords_t  coords) const noexcept { return _arr[get_index(coords.x, coords.y)]; }
			      reference operator[](coords_t  coords)       noexcept { return _arr[get_index(coords.x, coords.y)]; }
			const_reference at(size_type i)              const          { if (i >= size())                 { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[](i); }
			      reference at(size_type i)                             { if (i >= size())                 { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[](i); }
			const_reference at(size_type x, size_type y) const          { if (x >= _width || y >= _height) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			      reference at(size_type x, size_type y)                { if (x >= _width || y >= _height) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			const_reference at(coords_t coords)          const          { return at(coords.x, coords.y); }
			      reference at(coords_t coords)                         { return at(coords.x, coords.y); }

			// Arithmetic

			/*static T cross(const matrix_dyn& a, const matrix_dyn& b) noexcept { }//TODO
			static T dot(const matrix_dyn& a, const matrix_dyn& b) noexcept {}//TODO
			static T dot(const matrix_dyn& a, const matrix_dyn& b) noexcept {}//TODO*/

			const auto begin()   const noexcept { return _arr.begin(); }
			      auto begin()         noexcept { return _arr.begin(); }
			const auto end()     const noexcept { return _arr.end(); }
			      auto end()           noexcept { return _arr.end(); }
			const auto cbegin()  const noexcept { return _arr.cbegin(); }
			      auto cbegin()        noexcept { return _arr.cbegin(); }
			const auto cend()    const noexcept { return _arr.cend(); }
			      auto cend()          noexcept { return _arr.cend(); }
			const auto rbegin()  const noexcept { return _arr.rbegin(); }
			      auto rbegin()        noexcept { return _arr.rbegin(); }
			const auto rend()    const noexcept { return _arr.rend(); }
			      auto rend()          noexcept { return _arr.rend(); }
			const auto crbegin() const noexcept { return _arr.crbegin(); }
			      auto crbegin()       noexcept { return _arr.crbegin(); }
			const auto crend()   const noexcept { return _arr.crend(); }
			      auto crend()         noexcept { return _arr.crend(); }

#ifdef UTILS_COUT
			friend std::ostream& cout::operator<<(std::ostream& os, const matrix_dyn<T, MEMORY>& la);
#endif
		private:
			size_type _width;
			size_type _height;
			std::vector<T, Allocator> _arr;

#ifdef UTILS_COUT
			std::ostream& out(std::ostream& os) const
				{
				namespace ccu = cout::support;

				ccu::brace(os, "[");
				if constexpr (MEMORY == matrix_memory::width_first)
					{
					for (size_type i = 0; i < size() - 1; i++)
						{
						ccu::value(os, (*this)[i]);
						ccu::separ(os, ", ");
						if (i == width() - 1) { os << "\n "; }
						}
					}
				else
					{
					for (size_type y = 0; y < height() - 1; y++)
						{
						for (size_type x = 0; x < width(); x++)
							{
							ccu::value(os, (*this)[get_index(x, y)]);
							ccu::separ(os, ", ");
							}
						os << "\n ";
						}
					for (size_type x = 0; x < width() - 1; x++)
						{
						ccu::value(os, (*this)[get_index(x, height() - 1)]);
						ccu::separ(os, ", ");
						}

					}
				ccu::value(os, (*this)[size() - 1]);
				ccu::brace(os, "]");
				return os;
				}
#endif
		};
	}
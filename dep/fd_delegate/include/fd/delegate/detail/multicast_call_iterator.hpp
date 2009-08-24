#ifndef FD_DELEGATE_MULTICAST_CALL_ITERATOR
#define FD_DELEGATE_MULTICAST_CALL_ITERATOR

#include <algorithm>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>

namespace fd { namespace detail { namespace delegate
{

template<typename Function, typename Iterator>
class multicast_call_iterator
  : public boost::iterator_facade
  <
    multicast_call_iterator<Function, Iterator>,
    typename Function::result_type,
    boost::single_pass_traversal_tag,
    typename Function::result_type
  >
{
private:
  typedef boost::iterator_facade<
      multicast_call_iterator<Function, Iterator>,
      typename Function::result_type,
      boost::single_pass_traversal_tag,
      typename Function::result_type
    > base_type;
public:
  typedef typename Function::result_type result_type;

private:
  Iterator mutable iter_;
  Iterator end_;
  Function f_;
  boost::optional<result_type> mutable cache_;

public:
  multicast_call_iterator(Iterator iter, Iterator end, Function f)
    : iter_( iter ), end_( end ), f_( f )
  {
    iter_ = std::find_if( iter_, end_, is_callable() );
  }

private:
  friend class boost::iterator_core_access;

  typename base_type::reference dereference() const
  {
    if( !cache_.is_initialized() )
    {
      cache_.reset( f_( *iter_ ) );
    }
    return cache_.get();
  }

  void increment()
  {
    iter_ = std::find_if( ++iter_, end_, is_callable() );
    cache_.reset();
  }

  bool equal(multicast_call_iterator const & other) const
  {
    iter_ = std::find_if( iter_, end_, is_callable() );
    other.iter_ = std::find_if( other.iter_, other.end_, is_callable() );

    return iter_ == other.iter_;
  }

};  // class multicast_call_iterator

} } } // namespace fd::detail::delegate

#endif // FD_DELEGATE_MULTICAST_CALL_ITERATOR

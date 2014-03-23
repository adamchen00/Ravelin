/****************************************************************************
 * Copyright 2013 Evan Drumwright
 * This library is distributed under the terms of the GNU Lesser General Public 
 * License (found in COPYING).
 ****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
/// This file consists of general routines for constant matrices
//////////////////////////////////////////////////////////////////////////////

/// Gets a column iterator to a block
CONST_COLUMN_ITERATOR block_column_iterator_begin(unsigned row_start, unsigned row_end, unsigned col_start, unsigned col_end) const
{
  #ifndef NEXCEPT
  if (row_end < row_start || row_end > rows() || col_end < col_start || col_end > columns())
    throw InvalidIndexException();
  #endif

  CONST_COLUMN_ITERATOR i;
  i._count = 0;
  i._sz = (row_end - row_start)*(col_end - col_start);
  i._ld = leading_dim();
  i._rows = row_end - row_start;
  i._columns = col_end - col_start;
  i._data_start = i._current_data = data() + i._ld*col_start + row_start;
  return i;
}

/// Gets a column iterator to a block
CONST_COLUMN_ITERATOR block_column_iterator_end(unsigned row_start, unsigned row_end, unsigned col_start, unsigned col_end) const
{
  #ifndef NEXCEPT
  if (row_end < row_start || row_end > rows() || col_end < col_start || col_end > columns())
    throw InvalidIndexException();
  #endif

  CONST_COLUMN_ITERATOR i;
  i._sz = (row_end - row_start)*(col_end - col_start);
  i._count = i._sz;
  i._ld = leading_dim();
  i._rows = row_end - row_start;
  i._columns = col_end - col_start;
  i._data_start = data() + i._ld*col_start + row_start;
  i._current_data = i._data_start + i._sz;
  return i;
}

/// Gets a row iterator to a block
CONST_ROW_ITERATOR block_row_iterator_begin(unsigned row_start, unsigned row_end, unsigned col_start, unsigned col_end) const
{
  #ifndef NEXCEPT
  if (row_end < row_start || row_end > rows() || col_end < col_start || col_end > columns())
    throw InvalidIndexException();
  #endif

  CONST_ROW_ITERATOR i;
  i._count = 0;
  i._sz = (row_end - row_start)*(col_end - col_start);
  i._ld = leading_dim();
  i._rows = row_end - row_start;
  i._columns = col_end - col_start;
  i._data_start = i._current_data = data() + i._ld*col_start + row_start;
  return i;
}

/// Gets a row iterator to a block
CONST_ROW_ITERATOR block_row_iterator_end(unsigned row_start, unsigned row_end, unsigned col_start, unsigned col_end) const
{
  #ifndef NEXCEPT
  if (row_end < row_start || row_end > rows() || col_end < col_start || col_end > columns())
    throw InvalidIndexException();
  #endif

  CONST_ROW_ITERATOR i;
  i._sz = (row_end - row_start)*(col_end - col_start);
  i._count = i._sz;
  i._ld = leading_dim();
  i._rows = row_end - row_start;
  i._columns = col_end - col_start;
  i._data_start = data() + i._ld*col_start + row_start;
  i._current_data = i._data_start + i._sz;
  return i;
}

template <class V>
V& get_row(unsigned row, V& v) const
{
  #ifndef NEXCEPT
  if (row > rows())
    throw InvalidIndexException();

  if (sizeof(data()) != sizeof(v.data()))
    throw DataMismatchException();
  #endif

  // setup the iterators 
  CONST_ROW_ITERATOR source = block_row_iterator_begin(row, row+1, 0, columns());
  CONST_ROW_ITERATOR source_end = source.end();  
  v.resize(1, source._sz, false);
  ROW_ITERATOR target = v.row_iterator_begin();

  // iterate
  while (source != source_end)
    *target++ = *source++;

  return v;
}

template <class V>
V& get_column(unsigned column, V& v) const
{
  #ifndef NEXCEPT
  if (column > columns())
    throw InvalidIndexException();

  if (sizeof(data()) != sizeof(v.data()))
    throw DataMismatchException();
  #endif

  // setup the iterators 
  CONST_COLUMN_ITERATOR source = block_column_iterator_begin(0, rows(), column, column+1);
  CONST_COLUMN_ITERATOR source_end = source.end();  
  v.resize(source._sz, 1, false);
  COLUMN_ITERATOR target = v.column_iterator_begin();

  // iterate
  while (source != source_end)
    *target++ = *source++;

  return v;
}

/// Does the operation y = beta*y + alpha*this'*x' 
template <class T, class U>
U& transpose_mult_transpose(const T& x, U& y, REAL alpha=(REAL) 1.0, REAL beta=(REAL) 0.0) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(x.data()))
    throw DataMismatchException();
  if (sizeof(data()) != sizeof(y.data()))
    throw DataMismatchException();
  #endif

  unsigned xrows = x.rows();
  unsigned xcols = x.columns();
  #ifndef NEXCEPT
  if (xcols != this->rows())
    throw MissizeException();
  #endif
  y.resize(this->columns(), xrows);
  if (rows() == 0 || columns() == 0 || xrows == 0)
  {
    y.set_zero();
    return y;
  }
  CBLAS::gemm(CblasColMajor, CblasTrans, CblasTrans, columns(), xrows, rows(), alpha, data(), leading_dim(), x.data(), x.leading_dim(), beta, y.data(), y.leading_dim()); 
  return y;
}

/// Does the operation y = beta*y + alpha*this*x' 
template <class T, class U>
U& mult_transpose(const T& x, U& y, REAL alpha=(REAL) 1.0, REAL beta=(REAL) 0.0) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(x.data()))
    throw DataMismatchException();
  if (sizeof(data()) != sizeof(y.data()))
    throw DataMismatchException();
  #endif
  unsigned xrows = x.rows();
  unsigned xcols = x.columns();
  #ifndef NEXCEPT
  if (xcols != this->columns())
    throw MissizeException();
  #endif
  y.resize(this->rows(), xrows);
  if (rows() == 0 || columns() == 0 || xrows == 0)
  {
    y.set_zero();
    return y;
  }
  CBLAS::gemm(CblasColMajor, CblasNoTrans, CblasTrans, rows(), xrows, columns(), alpha, data(), leading_dim(), x.data(), x.leading_dim(), beta, y.data(), y.leading_dim()); 
      return y;
}

/// Does the operation y = beta*y + alpha*this'*x 
template <class T, class U>
U& transpose_mult(const T& x, U& y, REAL alpha=(REAL) 1.0, REAL beta=(REAL) 0.0) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(x.data()))
    throw DataMismatchException();
  if (sizeof(data()) != sizeof(y.data()))
    throw DataMismatchException();
  #endif

  unsigned xrows = x.rows();
  unsigned xcols = x.columns();
  #ifndef NEXCEPT
  if (xrows != this->rows())
    throw MissizeException();
  #endif
  y.resize(this->columns(), xcols);
  if (rows() == 0 || columns() == 0 || xcols == 0)
  {
    y.set_zero();
    return y;
  }
// TODO: q: why is this commented out?
//  if (xcols > 1)
    CBLAS::gemm(CblasColMajor, CblasTrans, CblasNoTrans, columns(), xcols, rows(), alpha, data(), leading_dim(), x.data(), x.leading_dim(), beta, y.data(), y.leading_dim()); 
/*
  else
    CBLAS::gemv(CblasTrans, rows(), columns(), alpha, data(), leading_dim(), x.data(), x.inc(), beta, y.data(), y.inc());
*/
  return y;
}

/// Does the operation y = beta*y + alpha*this*x 
template <class T, class U>
U& mult(const T& x, U& y, REAL alpha=(REAL) 1.0, REAL beta=(REAL) 0.0) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(x.data()))
    throw DataMismatchException();
  if (sizeof(data()) != sizeof(y.data()))
    throw DataMismatchException();
  #endif

  unsigned xrows = x.rows();
  unsigned xcols = x.columns();
  #ifndef NEXCEPT
  if (xrows != this->columns())
    throw MissizeException();
  #endif
  y.resize(rows(), xcols);
  if (rows() == 0 || columns() == 0 || xcols == 0)
  {
    y.set_zero();
    return y;
  }
// Q: why is this commented out?
//  if (xcols > 1)
    CBLAS::gemm(CblasColMajor, CblasNoTrans, CblasNoTrans, rows(), xcols, columns(), alpha, data(), leading_dim(), x.data(), x.leading_dim(), beta, y.data(), y.leading_dim()); 
/*
  else
    CBLAS::gemv(CblasNoTrans, rows(), columns(), (REAL) 1.0, data(), leading_dim(), x.data(), x.inc(), (REAL) 0.0, y.data(), y.inc());
*/
  return y;
}

/// Gets the specified sub matrix
/**
 * \param row_start the row to start (inclusive)
 * \param row_end the row to end (exclusive)
 * \param col_start the column to start (inclusive)
 * \param col_end the column to end (exclusive)
 * \param transpose determines whether to store the transpose of the submatrix into m
 * \return a (row_end - row_start) x (col_end - col_start) sized matrix
 */
template <class M>
M& get_sub_mat(unsigned row_start, unsigned row_end, unsigned col_start, unsigned col_end, M& m, Transposition trans = eNoTranspose) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  if (row_start > row_end || row_end > rows() || col_start > col_end || col_end > columns())
    throw InvalidIndexException();
  #endif

  // resize the matrix
  if (trans == eNoTranspose)
    m.resize(row_end - row_start, col_end - col_start);
  else
    m.resize(col_end - col_start, row_end - row_start);

  // see whether to exit now
  if (rows() == 0 || columns() == 0)
    return m;

  // copy each column using BLAS
  if (trans == eNoTranspose)
    for (unsigned i=0; i< m.columns(); i++)
      CBLAS::copy(m.rows(), data()+row_start+(col_start+i) * rows(), 1, m.data()+i*m.leading_dim(), 1);
  else
    for (unsigned i=0; i< m.rows(); i++)
      CBLAS::copy(m.columns(), data()+row_start+(col_start+i) * rows(), 1, m.data()+i, m.leading_dim());
  
  return m;
}

/// Gets a submatrix of columns (not necessarily a block)
/**
 * \param col_start an iterator pointing to the beginning of a container of column indices (container need not be sorted)
 * \param col_end an iterator pointing to the end of a container of column indices (container need not be sorted)
 * \param M contains the submatrix on return
 */
template <class ForwardIterator, class M>
M& select_columns(ForwardIterator col_start, ForwardIterator col_end, M& m) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  #endif

  // setup vectors of selections
  const unsigned ncols = std::distance(col_start, col_end);

  // resize matrix 
  m.resize(rows(), ncols);

  // make sure there is data to copy
  if (rows() == 0 || ncols == 0)
    return m;

  // populate m
  unsigned mi;
  ForwardIterator i;
  for (i=col_start, mi=0; i != col_end; i++, mi++)
  {
    assert(*i < columns());
    CBLAS::copy(rows(), data()+rows()*(*i), 1, m.data()+m.leading_dim()*mi, 1);
  }

  return m;
}

/// Gets a submatrix of columns (not necessarily a block)
/**
 * \param col_select a vector of bools; if the entry is true in the vector, the column is selected
 * \param M contains the submatrix on return
 */
template <class M>
M& select_columns(std::vector<bool>& col_select, M& m) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  if (col_select.size() != columns())
    throw MissizeException();
  #endif

  // setup vectors of selections
  const unsigned ncols = std::count(col_select.begin(), col_select.end(), true); 

  // resize matrix 
  m.resize(rows(), ncols);

  // make sure there is data to copy
  if (rows() == 0 || ncols == 0)
    return m;

  // populate m
  for (unsigned i=0, mi=0; i != columns(); i++)
    if (col_select[i])
    {
      CBLAS::copy(rows(), data()+rows()*i, 1, m.data()+rows()*mi, 1);
      mi++;
    }

  return m;
}

/// Gets a submatrix of rows (not necessarily a block)
/**
 * \param row_start an iterator pointing to the beginning of a container of row indices (container need not be sorted)
 * \param row_end an iterator pointing to the end of a container of row indices (container need not be sorted)
 * \param M contains the submatrix on return
 */
template <class ForwardIterator, class M>
M& select_rows(ForwardIterator row_start, ForwardIterator row_end, M& m) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  #endif

  // setup vectors of selections
  const unsigned nrows = std::distance(row_start, row_end);

  // resize matrix 
  m.resize(nrows, columns());

  // make sure there is data to copy
  if (nrows == 0 || columns() == 0)
    return m;

  // populate m
  unsigned mi;
  ForwardIterator i;
  for (i=row_start, mi=0; i != row_end; i++, mi++)
  {
    assert(*i < rows());
    CBLAS::copy(columns(), data()+*i, leading_dim(), m.data()+mi, m.leading_dim());
  }

  return m;
}

/// Gets a submatrix of rows (not necessarily a block)
/**
 * \param row_select a vector of bools; if the entry is true in the vector, the row is selected
 * \param M contains the submatrix on return
 */
template <class M>
M& select_rows(std::vector<bool>& row_select, M& m) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  if (row_select.size() != rows())
    throw MissizeException();
  #endif

  // setup vectors of selections
  const unsigned nrows = std::count(row_select.begin(), row_select.end(), true); 

  // resize matrix 
  m.resize(nrows, columns());

  // make sure there is data to copy
  if (nrows == 0 || columns() == 0)
    return m;

  // populate m
  for (unsigned i=0, mi=0; i != columns(); i++)
    if (row_select[i])
    {
      CBLAS::copy(columns(), data()+i, rows(), m.data()+mi, nrows);
      mi++;
    }

  return m;
}


/// Gets a submatrix (not necessarily a block)
/**
 * \param row_start an iterator pointing to the beginning of a container of row indices (container need not be sorted)
 * \param row_end an iterator pointing to the end of a container of row indices (container need not be sorted)
 * \param col_start an iterator pointing to the beginning of a container of column indices (container need not be sorted)
 * \param col_end an iterator pointing to the end of a container of column indices (container need not be sorted)
 * \param M contains the submatrix on return
 */
template <class ForwardIterator1, class ForwardIterator2, class X>
X& select(ForwardIterator1 row_start, ForwardIterator1 row_end, ForwardIterator2 col_start, ForwardIterator2 col_end, X& m) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  #endif

  // setup vectors of selections
  const unsigned nrows = std::distance(row_start, row_end);
  const unsigned ncols = std::distance(col_start, col_end);

  // resize matrix 
  m.resize(nrows, ncols);

  // make sure there is data to copy
  if (nrows == 0 || ncols == 0)
    return m;

  // get pointers to data
  REAL* mdata = m.data();
  const REAL* data = &operator()(*row_start, *col_start);

  // determine difference between first and last row
  unsigned rowbegin = *row_start;
  unsigned rowend = *row_start;
  for (ForwardIterator1 i = row_start; i != row_end; i++)
    rowend = *i;
  unsigned row_sub = rowend - rowbegin;

  // outer loop is over columns 
  for (ForwardIterator2 j=col_start; j != col_end; )
  {
    assert(*j < columns());
    for (ForwardIterator1 i = row_start; i != row_end; )
    {
      assert(*i < rows());

      // copy the data
      *mdata = *data;
      mdata++;

      // determine how we need to advance the rows
      if (i+1 != row_end)
      {
        data -= *i;
        i++;
        data += *i;
      }
      else
        i++;
    }

    // determine how we need to advance the columns
    unsigned col_diff = *(j+1) - *j;

    // advance data
    data += (col_diff * leading_dim());
    data -= row_sub; 
  }

  return m;
}

/// Gets a submatrix (not necessarily a block)
/*
 * \param rows a vector of row indices (need not be sorted)
 * \param cols a vector of column indices (need not be sorted) 
*/
template <class X>
X& select(const std::vector<bool>& rows, const std::vector<bool>& cols, X& m) const
{
  #ifndef NEXCEPT
  if (sizeof(data()) != sizeof(m.data()))
    throw DataMismatchException();
  if (rows.size() != this->rows() || cols.size() != this->columns())
    throw MissizeException();
  #endif

  // setup vectors of selections
  const unsigned nrows = std::count(rows.begin(), rows.end(), true);
  const unsigned ncols = std::count(cols.begin(), cols.end(), true);

  // resize matrix 
  m.resize(nrows, ncols);

  // make sure there is data to copy
  if (nrows == 0 || ncols == 0)
    return m;

  // get pointers to data
  COLUMN_ITERATOR mdata = m.column_iterator_begin();
  CONST_COLUMN_ITERATOR data = column_iterator_begin();

  // outer loop is over columns 
  for (unsigned j=0; j < cols.size(); j++)
  {
    for (unsigned i=0; i< rows.size(); i++, data++)
    {
      if (!cols[j] || !rows[i])
        continue;

      // copy the data
      *mdata++ = *data;
    }
  }

  return m;
}

/// Gets a square submatrix (not necessarily a block)
/*
 * \param indices a vector of row / column indices (need not be sorted)
*/
template <class M>
M& select_square(const std::vector<bool>& indices, M& result) const
{
  return select(indices, indices, result);
}

/**
 * \param start an iterator pointing to the beginning of a container of row/column indices (container need not be sorted)
 * \param end an iterator pointing to the end of a container of row/column indices (container need not be sorted)
 * \param M contains the submatrix on return
 */
template <class ForwardIterator, class M>
M& select_square(ForwardIterator start, ForwardIterator end, M& m) const
{
  return select(start, end, start, end, m);
}

/// Determines the transpose of a matrix and stores the result in a given matrix
/*
template <class M>
static M& transpose(const CONST_SHAREDMATRIXN& m, M& result)
{
  #ifndef NEXCEPT
  if (sizeof(m.data()) != sizeof(result.data()))
    throw DataMismatchException();
  #endif

  // resize the result
  result.resize(m.columns(), m.rows());

  const REAL* mdata = m.data();
  REAL* rdata = result.data();
  for  (unsigned i=0; i< m.rows(); i++)
    for (unsigned j=0; j< m.columns(); j++)
      rdata[i*result.leading_dim()+j] = mdata[j*m.leading_dim()+i];
  
  return result;
}

/// Multiplies the diagonal matrix formed from d by the matrix m
template <class V, class W>
static W& diag_mult(const V& d, const CONST_SHAREDMATRIXN& m, W& result)
{
  #ifndef NEXCEPT
  if (d.size() != m.rows())
    throw MissizeException();
  if (sizeof(d.data()) != sizeof(m.data()))
    throw DataMismatchException();
  #endif

  result.resize(d.size(), m.columns());
  for (unsigned i=0; i< m.columns(); i++)
    std::transform(d.begin(), d.end(), m.begin()+m.rows()*i, result.begin()+result.rows()*i, std::multiplies<REAL>());

  return result;
}

/// Multiplies the diagonal matrix formed from d by the matrix transpose(m)
template <class V, class W>
static W& diag_mult_transpose(const V& d, const CONST_SHAREDMATRIXN& m, W& result)
{
  #ifndef NEXCEPT
  if (d.size() != m.columns())
    throw MissizeException();
  if (sizeof(d.data()) != sizeof(m.data()))
    throw DataMismatchException();
  #endif

  // copy the transpose of m to the result
  CONST_SHAREDMATRIXN::transpose(m, result);

  // do the specified number of times
  for (unsigned i=0; i< m.rows(); i++)
    CBLAS::scal(d.size(), d[i], result.begin()+i, result.rows());

  return result;
}
*/

/// Multiplies the diagonal matrix formed from d by the vector v
template <class V, class U, class W>
static W& diag_mult(const V& d, const U& v, W& result)
{
  #ifndef NEXCEPT
  if (d.size() != v.size())
    throw MissizeException();
  if (sizeof(d.data()) != sizeof(v.data()))
    throw DataMismatchException();
  if (sizeof(d.data()) != sizeof(result.data()))
    throw DataMismatchException();
  #endif

  result.resize(d.size());
  std::transform(d.begin(), d.end(), v.begin(), result.begin(), std::multiplies<REAL>());
  return result;
}

/// Get an iterator to the beginning 
CONST_COLUMN_ITERATOR column_iterator_begin() const
{
  CONST_COLUMN_ITERATOR i;
  i._count = 0;
  i._sz = rows()*columns();
  i._ld = leading_dim();
  i._rows = rows();
  i._columns = columns();
  i._data_start = i._current_data = data();
  return i;
}

/// Get an iterator to the end
CONST_COLUMN_ITERATOR column_iterator_end() const
{
  CONST_COLUMN_ITERATOR i;
  i._sz = rows()*columns();
  i._count = i._sz;
  i._ld = leading_dim();
  i._rows = rows();
  i._columns = columns();
  i._data_start = data();
  i._current_data = data() + i._ld*i._columns;
  return i;
}

/// Get an iterator to the beginning 
CONST_ROW_ITERATOR row_iterator_begin() const
{
  CONST_ROW_ITERATOR i;
  i._count = 0;
  i._sz = rows()*columns();
  i._ld = leading_dim();
  i._rows = rows();
  i._columns = columns();
  i._data_start = i._current_data = data();
  return i;
}

/// Get an iterator to the end
CONST_ROW_ITERATOR row_iterator_end() const
{
  CONST_ROW_ITERATOR i;
  i._sz = rows()*columns();
  i._count = i._sz;
  i._ld = leading_dim();
  i._rows = rows();
  i._columns = columns();
  i._data_start = data();
  i._current_data = data() + i._ld*i._columns;
  return i;
}



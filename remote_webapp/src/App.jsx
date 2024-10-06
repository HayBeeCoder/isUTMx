import React from 'react'
import TestTables from './components/TestsTable'
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import TestPage from './components/TestPage';

const App = () => {
  return (
    <section>
      <Router>
        <Routes>
          <Route path="/" element={<TestTables data={data} />} />
          <Route path="/details/:id" element={<TestPage />} />
          </Routes >
      </Router >
    </section>
  )
}

export default App
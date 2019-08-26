/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libsolidity/ast/ASTCallgraph.h>

#include <libsolidity/ast/AST.h>
#include <boost/algorithm/string/join.hpp>
#include <json/json.h>

using namespace std;
using namespace langutil;

namespace dev
{
namespace solidity
{

void ASTCallgraph::print(ASTNode const& _ast, std::ostream& _stream)
{
	m_ostream = &_stream;
	_ast.accept(*this);
	m_ostream = nullptr;
}

bool ASTCallgraph::visit(ContractDefinition const& _node)
{
	m_activeContractName = _node.name();
	m_activeContractId = m_activeScopeId =  _node.id();
	m_idToSignatureMap[_node.id()] = _node.name() + ".constructor";
	return true;
}

bool ASTCallgraph::visit(InheritanceSpecifier const& _node)
{
	const UserDefinedTypeName* type = &_node.name();
	const ContractDefinition* contract =
		dynamic_cast<const ContractDefinition*>(type->annotation().referencedDeclaration);
	if (contract)
		m_callGraphEdges.emplace_back(m_activeScopeId, contract->id());
	return true;
}

bool ASTCallgraph::visit(FunctionDefinition const& _node)
{
	if (!_node.isConstructor())
	{
		m_idToSignatureMap[_node.id()] = m_activeContractName + "." + _node.externalSignature();
		m_activeScopeId = _node.id();
	}
	return true;
}

bool ASTCallgraph::visit(ModifierDefinition const& _node)
{
	m_idToSignatureMap[_node.id()] = m_activeContractName + "." + _node.name();
	m_activeScopeId = _node.id();
	return true;
}

bool ASTCallgraph::visit(EventDefinition const& _node)
{
	m_idToSignatureMap[_node.id()] = m_activeContractName + "." + _node.name();
	m_activeScopeId = _node.id();
	return true;
}

bool ASTCallgraph::visit(NewExpression const& _node)
{
	const UserDefinedTypeName* type =
		dynamic_cast<const UserDefinedTypeName*>(&_node.typeName());
	if (type) {
		const ContractDefinition* contract =
			dynamic_cast<const ContractDefinition*>(type->annotation().referencedDeclaration);
		if (contract)
			m_callGraphEdges.emplace_back(m_activeScopeId, contract->id());
	}
	return true;
}

bool ASTCallgraph::visit(MemberAccess const& _node)
{
	const CallableDeclaration* callable =
		dynamic_cast<const CallableDeclaration*>(_node.annotation().referencedDeclaration);
	if (callable)
		m_callGraphEdges.emplace_back(m_activeScopeId, callable->id());
	return true;
}

bool ASTCallgraph::visit(Identifier const& _node)
{
	const CallableDeclaration* callable =
		dynamic_cast<const CallableDeclaration*>(_node.annotation().referencedDeclaration);
	if (callable)
		m_callGraphEdges.emplace_back(m_activeScopeId, callable->id());
	return true;
}

void ASTCallgraph::endVisit(SourceUnit const&)
{
	for (pair<int, int> edge: m_callGraphEdges) {
		writeLine(m_idToSignatureMap[edge.first] + "->" + m_idToSignatureMap[edge.second]);
	}
	m_callGraphEdges.clear();
}

void ASTCallgraph::endVisit(ContractDefinition const&)
{
	m_activeContractName = "";
	m_activeContractId = m_activeScopeId = -1;
}

void ASTCallgraph::endVisit(FunctionDefinition const&)
{
	m_activeScopeId = m_activeContractId;
}

void ASTCallgraph::endVisit(ModifierDefinition const&)
{
	m_activeScopeId = m_activeContractId;
}

void ASTCallgraph::endVisit(EventDefinition const&)
{
	m_activeScopeId = m_activeContractId;
}

void ASTCallgraph::writeLine(string const& _line)
{
	*m_ostream << _line << endl;
}

}
}
